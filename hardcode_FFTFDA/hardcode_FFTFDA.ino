#include "arduinoFFT.h"

#define RELAY_PIN 3

#define SAMPLES 128
#define SAMPLE_RATE 250

arduinoFFT FFT = arduinoFFT();

const double vSample[SAMPLES] = {
0.0,
0.588,
0.951,
0.951,
0.588,
0.0,
-0.588,
-0.951,
-0.951,
-0.5877,
0.0,
0.588,
0.951,
0.951,
0.588,
0.0,
-0.588,
-0.951,
-0.951,
-0.588,
0.0,
0.588,
0.951,
0.951,
0.588,
0.0,
-0.588,
-0.951,
-0.951,
-0.588,
0.0,
0.588,
0.951,
0.951,
0.588,
0.0,
-0.588,
-0.951,
-0.951,
-0.588,
0.0,
0.588,
0.951,
0.951,
0.588,
0.0,
-0.588,
-0.951,
-0.951,
-0.588,
0.0,
0.588,
0.951,
0.951,
0.588,
0.0,
-0.588,
-0.951,
-0.951,
-0.588,
0.0,
0.588,
0.951,
0.951,
0.5877,
0.0,
-0.5877,
-0.951,
-0.951,
-0.5877,
0.0,
0.5877,
0.951,
0.951,
0.5877,
0.0,
-0.5877,
-0.951,
-0.951,
-0.5877,
0.0,
0.5877,
0.951,
0.951,
0.5877,
0.0,
-0.5877,
-0.951,
-0.951,
-0.5877,
0.0,
0.5877,
0.951,
0.951,
0.5877,
0.0,
-0.5877,
-0.951,
-0.951,
-0.5877,
0.0,
0.5877,
0.951,
0.951,
0.5877,
0.0,
-0.5877,
-0.951,
-0.951,
-0.5877,
0.0,
0.5877,
0.951,
0.951,
0.5877,
0.0,
-0.5877,
-0.951,
-0.951,
-0.5877,
0.0,
0.5877,
0.951,
0.951,
0.5877,
0.0,
-0.5877,
-0.951
};
double vReal[SAMPLES];
double vImag[SAMPLES];

unsigned long time_ = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  for (int i = 0; i < SAMPLES; i ++){
    vReal[i] = vSample[i];
    //Serial.println(vReal[i]);
  }
  
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

//    for (int i = 0; i < (SAMPLES / 2); i ++){
//      Serial.println(vReal[i]);
//    }
//
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLE_RATE);
    Serial.println(peak);
}
