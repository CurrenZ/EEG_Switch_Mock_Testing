import processing.serial.*;
import java.io.*;

Serial mySerial;
final String PORT = "COM5";
final int BAUDRATE = 115200;

final String FILE_PATH = "C:/Fake_Data_Test/receive.txt";

final int DECIMALS = 10000;

PrintWriter myWriter;

final int data_num = 128;
int cnt = 0;

void setup(){
  mySerial = new Serial(this, PORT, BAUDRATE);
  myWriter = createWriter(FILE_PATH);
}

void draw(){
}

// this part is executed, when serial-data is received
void serialEvent(Serial p) {
  try {
    //if (cnt >= data_num){
    //  exit();
    //}
    cnt ++;
    // get message till line break (ASCII > 13)
    String message = p.readStringUntil(13);
    // just if there is data
    if (message != null) {
      message = trim(message);
      float messageFloat = float(message);
      messageFloat /= DECIMALS;
      println("Received: "+ messageFloat);
       myWriter.println(messageFloat);
       myWriter.flush();
    }
  }
  catch (Exception e) {
  }
}
