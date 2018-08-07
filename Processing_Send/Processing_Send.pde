import processing.serial.*;
import java.io.*;

Serial mySerial;
final String PORT = "COM5";
final int BAUDRATE = 115200;

final String FILE_PATH = "C:/Fake_Data_Test/fakeSignal.txt";

String dataList[] = new String[9];

BufferedReader myReader;
String myLine;
 
 
void setup() {
  mySerial = new Serial(this, PORT, BAUDRATE);
  myReader = createReader(FILE_PATH);
}
 
void draw() {
  if (readDataLine()){
    float lineFloat = float(myLine);
    float temp = lineFloat * 100;
    int tempInt = (int)temp;
    ///println("now sending number: "+lineFloat);
    mySerial.write(Integer.toString(tempInt));
    // write any charcter that marks the end of a number
    if (lineFloat >= 0) mySerial.write('p');
    else if (lineFloat <0) mySerial.write('n');
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
      message = trim(message);
      float messageFloat = float(message);
      messageFloat /=100;
      println("message received: "+messageFloat);
    }
  }
  catch (Exception e) {
  }
}
