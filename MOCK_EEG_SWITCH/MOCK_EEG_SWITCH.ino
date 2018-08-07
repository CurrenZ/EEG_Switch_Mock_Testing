#include <Biquad.h>

// pins
#define DATA 3
#define SERIAL_AVAILABLE 2
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

int dataIndicator = LOW;
int serialIndicator = LOW;

bool frequencyMatch = false;

String SerialStr = "";

float channelPhase[SAMPLE_RATE] = {0};    // 2d array to store channel data

Biquad stopDC_filter = Biquad(bq_type_highpass, HP_CUTOFF / SAMPLE_RATE, FILTER_Q, PEAK_GAIN_DB);
Biquad notch_filter = Biquad(bq_type_notch, NOTCH_FREQ / SAMPLE_RATE, NOTCH_Q, PEAK_GAIN_DB);
Biquad AHP_bandpass_filter = Biquad(bq_type_bandpass, TARGET_FREQUENCY / SAMPLE_RATE, BP_Q, PEAK_GAIN_DB);
//stopDC_filter.calcBiquad();
//notch_filter.calcBiquad();
//AHP_bandpass_filter.calcBiquad();

void setup(){
	Serial.begin(115200);

	pinMode(DATA, OUTPUT);
  pinMode(SERIAL_AVAILABLE, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
	// initiating indicator for about 2 second
	for (int i = 0; i < 15; i ++){
		digitalWrite(DATA, HIGH);
    digitalWrite(SERIAL_AVAILABLE, LOW);
    digitalWrite(RELAY_PIN, LOW);
		delay(30);
		digitalWrite(DATA, LOW);
    digitalWrite(SERIAL_AVAILABLE, HIGH);
    digitalWrite(RELAY_PIN, LOW);
    delay(30);
    digitalWrite(DATA, LOW);
    digitalWrite(SERIAL_AVAILABLE, LOW);
    digitalWrite(RELAY_PIN, HIGH);
    delay(30);
	}
 digitalWrite(DATA, LOW);
 digitalWrite(RELAY_PIN, LOW);
 digitalWrite(SERIAL_AVAILABLE, LOW);
}

void loop(){
  for (int i = 0; i < SAMPLE_RATE; i ++){
    if (Serial.available()){
      smartDigitalWrite(SERIAL_AVAILABLE, HIGH, &serialIndicator);
      SerialStr = Serial.readString();
      float temp = 0.0f;
      if (SerialStr == "e"){
        Serial.println(temp);
      }
      else{
        temp = SerialStr.toFloat();
        channelPhase[i] = temp;
      }
      if (channelPhase[i] > 0.0){
        smartDigitalWrite(DATA, HIGH, &dataIndicator);
      } 
      else{
        smartDigitalWrite(DATA, LOW, &dataIndicator);
      }
    }
    else smartDigitalWrite(SERIAL_AVAILABLE, LOW, &serialIndicator);
  }
  if (frequencyMatched()){
    turnRelayOn();
  }
}

bool frequencyMatched(){
  bool result = false;
  float EEGuv = 0.0;
  for (int i = 0; i < SAMPLE_RATE; i++){
    float channelVal = 0.0f;
    float channelSum = 0.0f;
    channelVal = channelPhase[i];
    // recursive functions
    channelVal = stopDC_filter.process(stopDC_filter.process(channelVal));        // applying DC-blocking filter
    channelVal = notch_filter.process(notch_filter.process(channelVal));        // applying 60Hz notch filter
    channelVal = AHP_bandpass_filter.process(AHP_bandpass_filter.process(channelVal));  // applying band pass filter
    channelSum += channelVal * channelVal;                        // scaling the data
    EEGuv = (sqrt(abs(channelSum / SAMPLE_RATE)) * MICROVOLTS_PER_COUNT);
  }
  result = (EEGuv > CHANNEL_LOW);
  return result;
}

void turnRelayOn(){
  digitalWrite(RELAY_PIN, HIGH);
//  delay(RELAY_HOLD_TIME * 1000);
//  digitalWrite(RELAY_PIN, LOW);
}

void smartDigitalWrite(int pin, int targetStatus, int *currentStatus){
  if (targetStatus != *currentStatus){
    digitalWrite(pin, targetStatus);
    *currentStatus = targetStatus;
  }
}
