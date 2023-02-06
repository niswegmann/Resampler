#include "Resampler.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>

template <typename T> static constexpr T map
    (
        T x,
        T x_min, T x_max,
        T y_min, T y_max
    )
{
    //
    // Maps 'x' from the range '[x_min; x_max]' to the range '[y_min; y_max]'.
    //
    
    T const a = (x_max - x) / (x_max - x_min);
    T const b = (x - x_min) / (x_max - x_min);
    
    return a * y_min + b * y_max;
}

int main (int argc, const char * argv[])
{
    double const source_sample_rate = 11.0;
    double const target_sample_rate = 13.0;
    
    Resampler<2, 2> resampler (source_sample_rate, target_sample_rate);
    
    float source_input_buffer_l [1024], source_output_buffer_l [1024];
    float source_input_buffer_r [1024], source_output_buffer_r [1024];
    float target_input_buffer_l [1024], target_output_buffer_l [1024];
    float target_input_buffer_r [1024], target_output_buffer_r [1024];
    
    for (int index = 0; index < 1024; index++)
    {
        float const t = map<float>(index, 0, 1023, 0.0f, 10.0f);
        
        source_input_buffer_l[index] = sinf(t * 2.0f * M_PI * 1.0f);
        source_input_buffer_r[index] = cosf(t * 2.0f * M_PI * 1.0f);
    }
    
    std::array<int, 10> const sample_counts = { 3, 15, 5, 32, 80, 1, 75, 51, 33, 18 };
    
    int offset = 0;
    
    for (auto const sample_count : sample_counts)
    {
        float * const source_input_buffers [] = { source_input_buffer_l + offset, source_input_buffer_r + offset };
        float * const source_output_buffers [] = { source_output_buffer_l + offset, source_output_buffer_r + offset };
        float * const target_input_buffers [] = { target_input_buffer_l, target_input_buffer_r };
        float * const target_output_buffers [] = { target_output_buffer_l, target_output_buffer_r };
        
        printf("caller - processing %d samples\n", sample_count);
        
        resampler.process
        (
            source_input_buffers,
            source_output_buffers,
            target_input_buffers,
            target_output_buffers,
            sample_count,
            [](float * const * input_buffers, float * const * output_buffers, int sample_count)
            {
                printf("  callee - processing %d samples\n", sample_count);
                
                for (int sample_index = 0; sample_index < sample_count; sample_index++)
                {
                    output_buffers[0][sample_index] = tanhf(input_buffers[0][sample_index] * 2.0f);
                    output_buffers[1][sample_index] = tanhf(input_buffers[1][sample_index] * 2.0f);
                }
            }
        );
        
        offset += sample_count;
    }
    
    // `unaligned_output_buffer` will now contain the samples [1, 4, 9, 16, ...].
    
    printf("content of source output buffer:\n");
    
    for (int index = 0; index < offset; index++)
    {
        float const t = map<float>(index, 0, 1023, 0.0f, 10.0f);
        float const y_l = source_output_buffer_l[index];
        float const y_r = source_output_buffer_r[index];
        
        printf("  %f, %f, %f\n", t, y_l, y_r);
    }
    
    return EXIT_SUCCESS;
}
