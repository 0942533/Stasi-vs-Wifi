///////////////// 1. LIGHTS WITH MOTION SENSOR /////////////////
// Include Wifi functions
#include <ESP8266WiFi.h>

// to connect to the WiFi
const char* ssid = "wifiname";
const char* password = "wifipassword";
 
WiFiServer server(80);

//The time the sensor gets to calibrate
int calibrationTime = 10;       
 
//The time when the sensor outputs a low impulse
long unsigned int lowIn;        
 
//The amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
long unsigned int pause = 5000; 
 
boolean lockLow = true;
boolean takeLowTime; 
 
int pirPin = 12; //Pin D6 for Sensor
int ledPin = 13; //Pin D7 for Led
 
void setup(){
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect with WiFi
  WiFi.begin(ssid, password);

  // While not connected place " . "
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
    
  }
 
void loop(){

   // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  //Refresh page as much as possible
  client.println("<meta http-equiv=\"refresh\" content=\"0\">");
  client.println("</head>");
 
  client.print("Led pin is now: ");


     //If there has been detected motion
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);
       client.print("On");
       if(lockLow){ 
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;           
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec");
         delay(50);
         }        
         takeLowTime = true;
       }
 
     if(digitalRead(pirPin) == LOW){      
       digitalWrite(ledPin, LOW);
       client.print("Off");  
 
       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause,
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){ 
           //makes sure this block of code is only executed again after
           //a new motion sequence has been detected
           lockLow = true;                       
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }
       client.println("<br><br>");
//  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
//  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");
  client.println("<br><br>");  
  // Places the security cam on the page
//  client.print("<iframe width=640 height=360 src=");
//  client.print("http://192.168.1.101/picture/2/frame/"); 
//  client.println(" frameborder=0 allowfullscreen></iframe>");
  client.println("</html>");
  delay(1);
 
  }
  
/////////////////// 2. DOOR OPEN/CLOSING WITH SERVO MOTOR /////////////////
//#include <Servo.h>
//
//const byte button = 4; //button pin, connect to ground to move servo
//
//Servo servo;
//
//boolean buttonState = HIGH;
//boolean lastButtonState = HIGH;
//int pos = 0;
//
//void setup()
//{
//   pinMode(button, INPUT_PULLUP); //arduino monitor pin state
//   servo.attach(9); //pin for servo control signal
//}
//
//void loop()
//{
//   buttonState = digitalRead(button);
//   if (buttonState != lastButtonState)
//   {
//      if (buttonState == LOW)
//      {
//        //Open the door
//        for(pos = 80; pos <= 180; pos +=1){
//          servo.write(pos);
//          delay(15);
//        }
//         //A brake between opening en closing the door
//         delay(3000);  
//
//        //Closing the door
//        for(pos = 180; pos >= 80; pos -=1){
//          servo.write(pos);
//          delay(15);
//        }
//      } 
//      lastButtonState = buttonState;
//   }
//}
