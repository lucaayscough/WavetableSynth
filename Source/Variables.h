#pragma once


struct Variables
{
    static constexpr double parameterSmoothingTime = 0.000001;          // Amount of time to smooth value.
    static constexpr int numVoices = 16;                                // Number of oscillators assigned to each synth voice.
    static constexpr int resampleCoefficient = 4;                       // Upsample coefficient used to reample audio.
    static constexpr int numResampleFilters = 4;                        // Number of filters used to remove aliasing from upsampled audio.
};
