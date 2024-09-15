#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SAMPLE_RATE 44100
#define TABLE_SIZE 10000
#define FREQUENCY 440.0
#define AMPLITUDE 0.5
#define DURATION 5  // Duration in seconds

double cos_table[TABLE_SIZE];

void init_cos_table() {
    FILE *file = fopen("cosine_values.csv", "w");
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }
    fprintf(file, "Index,CosineValue\n");  // CSV header
    for (int i = 0; i < TABLE_SIZE; i++) {
        cos_table[i] = cos((2 * M_PI * i) / TABLE_SIZE);
        fprintf(file, "%d,%lf\n", i, cos_table[i]);  // Write index and cos value
    }
    fclose(file);
    printf("Cosine values written to cosine_values.csv\n");
}

double lookup_cos(double x) {
    int index = (int)((x / (2 * M_PI)) * TABLE_SIZE) % TABLE_SIZE;
    return cos_table[index];
}

static int audio_callback(const void* inputBuffer, void* outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags, void* userData) {
    float* out = (float*)outputBuffer;
    static double phase = 0.0;
    double phaseIncrement = (2.0 * M_PI * FREQUENCY) / SAMPLE_RATE;

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        *out++ = (float)(AMPLITUDE * lookup_cos(phase));  // Output the cosine wave
        phase += phaseIncrement;
        if (phase >= 2.0 * M_PI) {
            phase -= 2.0 * M_PI;
        }
    }

    return paContinue;
}

int main() {
    PaError err;

    err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio errror: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    init_cos_table();

    PaStream* stream;
    err = Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, SAMPLE_RATE, 256, audio_callback, NULL);
    if (err != paNoError) {
        printf("PortAudio errror: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        printf("PortAudio errror: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    Pa_Sleep(DURATION* 1000);

    err = Pa_StopStream(stream);
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    Pa_Terminate();

    printf("Sound playback finished.\n");

    return 0;

}
