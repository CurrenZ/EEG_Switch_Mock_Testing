#include <Filters.h>

// pin(s)
#define RELAY_PIN 3

#define SAMPLE_RATE 250 // sampling rate of OpenBCI

// for Serial Communication
#define DECIMALS 10000.0f
long rawSerial;
long rawData=0;
int dataCnt = 0;

// parameters for filtering
const float TARGET_FREQUENCY = 25.0f;
const float HP_FREQUENCY = 10.0f;
const float LP_FREQUENCY = 25.0f;
const float QUALITY_FACTOR = 1.0f;
const float INITIAL_VALUE = 0.0f;

float inVal = 0.0f;
float hold = 99999.0f;

// creating a low pass filter
FilterOnePole HPF(HIGHPASS, HP_FREQUENCY, INITIAL_VALUE);
// FilterOnePole LPF(LOWPASS, LP_FREQUENCY, INITIAL_VALUE);
FilterTwoPole LPF(LP_FREQUENCY, QUALITY_FACTOR, INITIAL_VALUE);

void setup() {
	Serial.begin(115200);
  LPF.setAsFilter(LOWPASS_BESSEL, LP_FREQUENCY, INITIAL_VALUE);

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
		if (Serial.available()) myRead(&rawSerial, &rawData, &inVal);
}

void myRead(long *rS, long *rD, float *channelData){
  *rS = Serial.read();
  // handle digits
  if ((*rS >= '0') && (*rS <= '9')) {
    *rD = 10 * *rD + *rS - '0';
  }
  // handle delimiter
  else if ((*rS == 'n') || (*rS == 'p')){
    if (*rS == 'n'){
      *rD *= -1;
    }
    //Serial.println(*rD);
    *channelData = *rD / DECIMALS;
    float outVal = HPF.input(*channelData);
    outVal = LPF.input(outVal);
    long temp = (long)(outVal * DECIMALS);
    Serial.println(temp);
    *rD = 0;
  }
}
