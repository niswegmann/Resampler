CFLAGS=-std=c++14 -I ..

Resampler-Example: Resampler-Example.cpp ../Resampler.hpp ../Resampler-Cpp.inl ../Resampler-NEON.inl
	$(CC) Resampler-Example.cpp -o Resampler-Example $(CFLAGS)

.PHONY: all

all: Resampler-Example

.PHONY: clean

clean:

	rm Resampler-Example
