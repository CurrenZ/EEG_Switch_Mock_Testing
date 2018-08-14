import java.io.*;

final String FILE_PATH = "C:/Fake_Data_Test/filteredSine25.txt";
final String RESULT_PATH = "C:/Fake_Data_Test/resultSine25Hz.txt";
BufferedReader myReader;
PrintWriter myWriter;
String myLine;
int highCnt = 0;
int lineNum = 0;
long sum = 0;

void setup(){
  myReader = createReader(FILE_PATH);
  myWriter = createWriter(RESULT_PATH);
}

void draw(){
  if (readDataLine()){
    println(myLine);
    float data = float(myLine);
    lineNum ++;
    sum += data;
    if (data >= 5.0) highCnt ++;
  }
  else myExit();
}

boolean readDataLine(){
  try{
    myLine = myReader.readLine();
  }catch(IOException e){
    e.printStackTrace();
    myLine = null;
  }
  //if (myLine == "end") exit();
  return (myLine != null);
}

void myExit(){
  println("Finish!" + highCnt);
  myWriter.println("Total number of signal high is: " + highCnt + "/" + lineNum + " = " + ((float(highCnt))/(float(lineNum))*100.0f) + "%");
  myWriter.println("Average amplitude of the signal is: " + ((sum * 1.0f)/(lineNum * 1.0f)));
  myWriter.flush();
  myWriter.close();
  exit();
}
