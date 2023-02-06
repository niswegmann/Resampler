#  Resampler

*Resampler* is a small header-only C++ library for real-time sample rate
conversion with a fractional ratio.

Here's and example of how to use it. Say we want to convert from 44.1 Hz to 48
Hz in stereo.

Start by creating a `Resampler` instance with 2 input and 2 output channels and store it
somewhere.

    double const source_sample_rate = 44100, target_sample_rate = 48000;
    
    Resampler<2, 2> resampler (source_sample_rate, target_sample_rate);

Allocate your target buffers (for processing the signal at 48 kHz):

    float * const target_input_buffers [2] =
    {
        malloc(max_target_sample_count * sizeof(float)),
        malloc(max_target_sample_count * sizeof(float))
    };
    
    float * const target_output_buffers [2] =
    {
        malloc(max_target_sample_count * sizeof(float)),
        malloc(max_target_sample_count * sizeof(float))
    };

Make sure that `max_target_sample_count` is greater than or equal to
`ceil(max_source_sample_count * target_sample_rate / source_sample_rate)`,
where `max_source_sample_count` is the maximum sample count for the source
buffers (that contains the provided signal at 44.1 kHz).

In your process callback, use the resampler to process the 44.1 kHz signal at
48 kHz instead.
    
    ...
    
    resampler.process
    (
        source_input_buffers,
        source_output_buffers,
        target_input_buffers,
        target_output_buffers,
        sample_count,
        [](float * const input_buffers [2], float * const output_buffers [2], int sample_count)
        {
            ... // Do your DSP on the resampled buffers.
        }
    );
    
    ...

`Resampler` also works the other way around, if you want to process a 48 kHz
signal at 44.1 kHz.

Internally, `Resampler` uses a 3rd-order polynomial (cubic hermite) interpolator
on a 4-point window. A slightly more complicated version of linear interpolation.
Quality-wise, `Resampler` works best if you oversample the source signal first
by a factor of 4, 8 or 16, depending on your needs.
