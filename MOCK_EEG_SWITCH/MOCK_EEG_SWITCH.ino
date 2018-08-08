#include <Biquad.h>

// pins
#define RELAY_PIN 11

#define RELAY_HOLD_TIME 1         // in second(s)

// parameters for filters
#define FILTER_Q 0.5f               // 0.707 (Butterworth) when critically damped
#define NOTCH_Q 4.0f            // sharp notch
#define BP_Q 2.0f             // step slope
#define PEAK_GAIN_DB 0.0f         // don't want any gain in banpass filter
#define MICROVOLTS_PER_COUNT 0.02235174f  //
#define HP_CUTOFF 0.5f            // 
#define NOTCH_FREQ 60.0f          // power line frequency
#define TARGET_FREQUENCY 25.0f         // the frequency that to be detected
#define CHANNEL_LOW 5.0f          // when then activity in such channel is low

#define NUM_CHANNEL 1    // number of signals needed from the brains
#define SAMPLE_RATE 250  // sampling rate used by OpenBCI

// for Serial Communication
#define DECIMALS 10000.0f

// for Serial Read
int rawSerial;
int rawData=0;

// for frequency detection
bool frequencyMatch = false;
float channelPhase[SAMPLE_RATE] = {};    // 2d array to store channel data

Biquad stopDC_filter = Biquad(bq_type_highpass, HP_CUTOFF / SAMPLE_RATE, FILTER_Q, PEAK_GAIN_DB);
Biquad notch_filter = Biquad(bq_type_notch, NOTCH_FREQ / SAMPLE_RATE, NOTCH_Q, PEAK_GAIN_DB);
Biquad AHP_bandpass_filter = Biquad(bq_type_bandpass, TARGET_FREQUENCY / SAMPLE_RATE, BP_Q, PEAK_GAIN_DB);
//stopDC_filter.calcBiquad();
//notch_filter.calcBiquad();
//AHP_bandpass_filter.calcBiquad();
 
void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  
  // initiating indicator for about 2 second
  for (int i = 0; i < 20; i ++){
    digitalWrite(RELAY_PIN, LOW);
    delay(50);
    digitalWrite(RELAY_PIN, HIGH);
    delay(50);
  }
 digitalWrite(RELAY_PIN, LOW);
}
 
void loop() {
  // read serial-data, if available
//  for (int i = 0; i < SAMPLE_RATE; i++){
//    if (Serial.available()) {
//      myRead(&rawSerial, &rawData, &channelPhase[i]);
//    }
//  }
  int dataCnt = 0;
  while(dataCnt < SAMPLE_RATE){
    if (Serial.available()) {
      myRead(&rawSerial, &rawData, &channelPhase[dataCnt]);
      dataCnt ++;
    }
  }
  if (frequencyMatched()){
    digitalWrite(RELAY_PIN, HIGH);
    delay(2000);
  }
}

void myRead(int *rS, int *rD, float *channelData){
  *rS = Serial.read();
  // handle digits
  if ((*rS >= '0') && (*rS <= '9')) {
    *rD = 10 * *rD + *rS - '0';
  }
  // handle delimiter
  else{
    if (*rS == 'n'){
      *rD *= -1;
    }
    if (*rD !=0){
      //Serial.println(*rD);
      *channelData = *rD/DECIMALS;
      //Serial.println(*channelData * DECIMALS);
    } 
    *rD = 0;
  }
}

void smartDigitalWrite(int pin, int targetStatus, int *currentStatus){
  if (targetStatus != *currentStatus){
    digitalWrite(pin, targetStatus);
    *currentStatus = targetStatus;
  }
}

bool frequencyMatched(){
  bool result = false;
  float EEGuv = 0.0;
  float channelVal = 0.0f;
  float channelSum = 0.0f;
  for (int i = 0; i < SAMPLE_RATE; i++){
    channelVal = channelPhase[i];
    // recursive functions
    channelVal = stopDC_filter.process(stopDC_filter.process(channelVal));        // applying DC-blocking filter
    channelVal = notch_filter.process(notch_filter.process(channelVal));        // applying 60Hz notch filter
    channelVal = AHP_bandpass_filter.process(AHP_bandpass_filter.process(channelVal));  // applying band pass filter
    channelSum += channelVal * channelVal;                        // scaling the data    
  }
  EEGuv = (sqrt(abs(channelSum / SAMPLE_RATE)) * MICROVOLTS_PER_COUNT);
  int temp = EEGuv * DECIMALS;
  Serial.println(temp);
  result = (EEGuv > CHANNEL_LOW);
  return result;
}
