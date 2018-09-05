#include "arduinoFFT.h"
#include <Filters.h>
#include "sampleData.h"
 
#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 250 //Hz, must be less than 10000 due to ADC
#define SCALE 250.0f/128.0f
#define PEAK_TSHD 10.0f

#define DECIMALS 10000.0f

const int SIZE_TARGET_FREQ = 4;
const float TARGET_FREQ[SIZE_TARGET_FREQ] = {25.0, 30.0, 40.0, 50.0};

arduinoFFT FFT = arduinoFFT();

double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
    Serial.begin(115200);
}
 
void loop() {
  for (int i = 0; i < SAMPLES; i ++){
    vReal[i] = SAMPLE_DATA[i];
    vImag[i] = 0;
  }
  
  /*FFT*/
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_BLACKMAN, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);


  Serial.println(1111);
  Serial.println(1111);
  Serial.println(1111);
  for (int i = 0; i < (SAMPLES / 2); i ++){
    long tmp = (long)(vReal[i] * DECIMALS);
    Serial.println(tmp);
  }
  Serial.println(1111);
  Serial.println(1111);
  Serial.println(1111);
  // finding peaks
//  for (int i = 0; i < SAMPLES / 2; i ++){
//    if (vReal[i] > PEAK_TSHD){
//      float freq = i * SCALE;
//      for (int pos = 0; pos < SIZE_TARGET_FREQ; pos ++){
//        if (apxEqual(TARGET_FREQ[pos], freq)){
//          Serial.println("Frequency " + String(TARGET_FREQ[pos]) + "Hz" + " is detected as " + String(freq) + "Hz with magnitude " + String(vReal[i]));
//        }
//      }
//      Serial.println("------------------------------------------------------------------------------------");
//    }
//  }
//  delay(30000);
}

bool apxEqual(float a, float b){
  int dif = (int)(a-b);
  return (abs(dif)<=1);
}













