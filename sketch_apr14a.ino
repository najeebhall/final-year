#include <SoftwareSerial.h>
#include <SPI.h>
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/Arial14.h>


SoftDMD dmd(1,1);
// Number of P10 panels used X, Y
//DMD_TextBox box(dmd, 2, 1, 32, 16); 
// Set Box (dmd, x, y, Height, Width) 
SoftwareSerial ESPSerial(2, 3); // RX, TX
String inData; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout
//Serial1.begin(115200);
void setup() {
  // Open serial communications
  Serial.begin(57600);
  //Serial1.begin(115200);
  ESPSerial.begin(57600);
  // set the data rate for the SoftwareSerial port
  // if this doesnt work you for you then try different baud rate
  //ESPSerial.begin(115200);
  //ESPSerial.println("Ready to take AT commands");
  sendToWifi("AT+CIPMUX=1",responseTime,DEBUG); // configure for multiple connections
  //delay(1000);
  sendToWifi("AT+CIPSERVER=1,80",responseTime,DEBUG); // turn on server on port 80
  //ESPSerial.println("AT+CIPMUX=1");
 // delay(1000);
  //ESPSerial.println("AT+CIPSERVER=1,80");
  dmd.setBrightness(10); // Set brightness 0 - 255 
  dmd.selectFont(SystemFont5x7); // Font used
  //dmd.begin();     // Start DMD 
  //dmd.fillScreen(true);
}
char rx=0;
String str,text,timer;
long timer2,timer3,time_passed=0;
int x,y,z ;
void loop() { // run over and over
  //dmd.end();
  while (ESPSerial.available() > 0)
    {
        char recieved = ESPSerial.read();
        inData += recieved; 

        // Process message when new line character is recieved
        if (recieved == '\n')
        {
            //Serial.print("Arduino Received: ");
            Serial.print(inData);
            x=inData.indexOf("p");
            //y=inData.indexOf("0");
            if(x!=-1){
              text=inData.substring(x+1);
              y=text.indexOf("0");
              if (y!=-1){
                z=text.indexOf("*");
                timer=text.substring(z+1,y);
                Serial.println(timer2);
                timer2=timer.toInt() * 1000;
                text=text.substring(0,z);
                //text.trim();
                if (text.length() <6){
                  DMD_TextBox box(dmd, 2, 1, 32, 16);
                  dmd.begin();
                  dmd.clearScreen();
                  dmd.fillScreen(true);
                  dmd.setBrightness(10);
                  box.print(text);
                  delay(timer2);
                  dmd.clearScreen();
                  dmd.fillScreen(true);
                  dmd.setBrightness(255); 
                  dmd.end();
                }
                else{
                  DMD_TextBox box(dmd, 1, 4, 32, 16);
                  int len=text.length();
                  timer3=timer.toInt()/(len/2);
                  Serial.println(timer3);
                  Serial.println("timer");
                  dmd.begin();
                  dmd.selectFont(Arial14); // Font used
                  dmd.setBrightness(10);
                  dmd.clearScreen();
                  dmd.fillScreen(true);
                 // const char *next=text.c_str();
                  while (time_passed<timer3){
                    char *next=text.c_str();
                    while(*next) {
                    box.print(*next);
                    delay(500);
                    next++;
                    }
                    time_passed+=1;
                    //delay(time_passed); 
                  }
                  dmd.clearScreen();
                  dmd.fillScreen(true);
                  dmd.setBrightness(255); 
                  dmd.end();
                }
              }
              //dmd.end();
            }
            // You can put some if and else here to process the message juste like that:  
            inData = ""; // Clear recieved buffer
        }
    }
  if (Serial.available()) {
    rx = Serial.read();
    ESPSerial.print(rx);
  }
}

String sendToWifi(String command, const int timeout, boolean debug){
  String response = "";
  ESPSerial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(ESPSerial.available())
    {
    // The esp has data so display its output to the serial window 
    char c = ESPSerial.read(); // read the next character.
    response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}
