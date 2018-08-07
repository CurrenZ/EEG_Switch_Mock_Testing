import processing.serial.*;
import java.io.*;

Serial mySerial;
final String PORT = "COM5";
final int BAUDRATE = 115200;

final String FILE_PATH = "C:/Fake_Data_Test/fakeSignal.txt";

String dataList[] = new String[9];

BufferedReader myReader;
String myLine;


void setup(){
	mySerial = new Serial(this, PORT, BAUDRATE);
	//mySerial.bufferUntil('\n');

	myReader = createReader(FILE_PATH);
}

void draw(){
	if (readDataLine()){
		//dataList = split(myLine, ',');
  //  String dataStr = dataList[1];
		//println(myLine);
		mySerial.write(myLine);
    mySerial.write("e");
	}
}

boolean readDataLine(){
	try{
		myLine = myReader.readLine();
	}catch(IOException e){
		e.printStackTrace();
		myLine = null;
	}
	return !(myLine == null);
}

// this part is executed, when serial-data is received
void serialEvent(Serial p) {
  try {
    // get message till line break (ASCII > 13)
    String message = p.readStringUntil(13);
    // just if there is data
    if (message != null) {
      println("message received: "+trim(message));
    }
  }
  catch (Exception e) {
  }
}
