import processing.serial.*;
import java.io.*;

final String FILE_PATH = "C:/Fake_Data_Test/";
final String FILE_NAME = "test_signal";

final String FILE_ORIG = FILE_PATH + FILE_NAME + ".txt";
final String FILE_TRMD = FILE_PATH + FILE_NAME + "_TRMD.txt";

final float DECIMALS = 10000.0f;

BufferedReader myReader;
PrintWriter myWriter;
String myLine = "";

void setup(){
  myReader = createReader(FILE_ORIG);
  myWriter = createWriter(FILE_TRMD);
}

void draw(){
  if (readDataLine()){
    float lineFloat = float(myLine);
    long tmp = (long)(lineFloat * DECIMALS);
    float toWrite = tmp / DECIMALS;
    myWriter.println(toWrite);
    myWriter.flush();
  }
  else exit();
}

boolean readDataLine(){
  try{
    myLine = myReader.readLine();
  }catch(IOException e){
    e.printStackTrace();
    myLine = null;
  }
  return (myLine != null);
}
