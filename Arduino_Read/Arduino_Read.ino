#define DATA 3
#define SERIAL_AVAILABLE 2

String SerialStr = "";
float channelData = 5.0;

void setup(){
	Serial.begin(115200);

	pinMode(DATA, OUTPUT);
  pinMode(SERIAL_AVAILABLE, OUTPUT);
	// initiating indicator for 2 second
	for (int i = 0; i < 20; i ++){
		digitalWrite(DATA, HIGH);
    digitalWrite(SERIAL_AVAILABLE, LOW);
		delay(50);
		digitalWrite(DATA, LOW);
    digitalWrite(SERIAL_AVAILABLE, HIGH);
		delay(50);
	}
}

void loop(){
  if (Serial.available()){
    digitalWrite(SERIAL_AVAILABLE, HIGH);
    SerialStr = Serial.readString();
    channelData = SerialStr.toFloat();
    delay(500);
    if (channelData > 0.0){
      digitalWrite(DATA, HIGH);
      delay(250);
    } 
    else{
      digitalWrite(DATA, LOW);
      delay(250);
    }
  }
  else{
    digitalWrite(SERIAL_AVAILABLE, LOW);
  }
}
