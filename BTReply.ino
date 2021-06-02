#include <SoftwareSerial.h>
#include <SD.h>

const int CS_PIN = 10;
bool isSDCardInitialized = false;

SoftwareSerial SW_Serial(8,9); // RX, TX

// we will always consider a zero as no command
byte command = 0;

void setup()
{
  // Prepping SD Card component
  //CS pin must be configured as an output
  pinMode(CS_PIN, OUTPUT);
  initSDCard();
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }


  Serial.println("Ready for output...");

  // set the data rate for the SoftwareSerial port
  SW_Serial.begin(38400);
  SW_Serial.println("Hello, world?");

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
}

void loop()
{
  // always initialize the command back to 0
  command = 0;  
  while (SW_Serial.available()){
    command = SW_Serial.read();
  }
  switch (command){
    case 49: { // ASCII char 1
      SW_Serial.println("This is a basic message sent over BT.");
      break;
    }
    case 50: { // ASCII char 2
      SW_Serial.println(random(50000));
      break;
    }
    case 51: { // ASCII char 3
      if (isSDCardInitialized){
        String output = "";
        SW_Serial.print("Retrieving file...\n");
        File dataFile = SD.open("test.txt", FILE_READ);
        if (dataFile) { 
        while (dataFile.available()) { //execute while file is available
          char letter = dataFile.read(); //read next character from file
          // I strip off the 13 found on each line of the 
          // text file, then I put just one back on (below 
          // in the SW_Serial.prinln(output) so that the SoBtEx Android
          // app will recognize the end of transmission. It's odd
          // but it works.
          if (letter != 13){
            output.concat(letter);
          }
        }
        SW_Serial.println(output); //display all
        //SW_Serial.println("");
        dataFile.close(); //close file
        }
      }
      else{
        SW_Serial.println("There is no SD Card in the device.\nOr the device is not available.");
      }
      break;
    }
    case 52: { // ASCII char 4
      File dataFile = SD.open("test.txt", FILE_WRITE);
       if (dataFile)
       {
        dataFile.print(random(50000));
        dataFile.print(",");
        dataFile.print(random(50000));
        dataFile.print(",");
        dataFile.println(random(50000));
        dataFile.close(); //Data isn't written until we run close()!
        SW_Serial.println("Writing some data to the file: test.txt");
       }
      
      break;
    }
  }
  if (command != 0){
    Serial.println("got : " + (char)command);
  }

  
  //Serial.write(SW_Serial.read());
  if (Serial.available())
    SW_Serial.write(Serial.read());
}

void initSDCard(){
 if (isSDCardInitialized){
  return;
 }
 
 if (SD.begin(CS_PIN))
 {
   isSDCardInitialized = true;
 }
}
