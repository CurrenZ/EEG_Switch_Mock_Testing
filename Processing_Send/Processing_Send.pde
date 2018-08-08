import processing.serial.*;
import java.io.*;

Serial mySerial;
final String PORT = "COM5";
final int BAUDRATE = 115200;

final String FILE_PATH = "C:/Fake_Data_Test/fakeSignal.txt";
final String FILTERED_DATA_PATH = "C:/Fake_Data_Test/filteredSignal.txt";

final int DECIMALS = 10000;

String dataList[] = new String[9];

BufferedReader myReader;
PrintWriter myWriter;
String myLine;
long lineCnt = 15000;
 
 
void setup() {
  mySerial = new Serial(this, PORT, BAUDRATE);
  myReader = createReader(FILE_PATH);
  myWriter = createWriter(FILTERED_DATA_PATH);
}
 
void draw() {
  if (readDataLine()){
    if (lineCnt < 0){
      myWriter.flush();
      myWriter.close();
      exit();
    } 
    float lineFloat = float(myLine);
    float temp = lineFloat * DECIMALS;
    int tempInt = (int)temp;
    ///println("now sending number: "+lineFloat);
    mySerial.write(Integer.toString(tempInt));
    // write any charcter that marks the end of a number
    if (lineFloat >= 0) mySerial.write('p');
    else if (lineFloat <0) mySerial.write('n');
    lineCnt --;
    if ((lineCnt%250) == 0) println("Lines remain --------------------------------------------- " + lineCnt);
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
      myWriter.println(message);
      myWriter.flush();
      println("Wrote to file: " + message);
      float messageFloat = float(message);
      messageFloat /= DECIMALS;
      println("message received: "+messageFloat);
      
    }
  }
  catch (Exception e) {
  }
}
