
#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;
uint8_t frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
#define NUMBER_OF_IMAGE_LINES 240
#define IMAGE_DATA_LENGTH 320*NUMBER_OF_IMAGE_LINES*2

#include "cloudConnection.h"


#include "HX711.h"
#include "DHT.h"

#define REPLAY_PIN 9
#define scaleDataPin 7
#define scaleClockPin 8
HX711 h;

#define MAX_WEIGHT 10.0
#define MIN_WEIGHT 2.0

#define DHTTYPE DHT11
#define DHTPIN 10
DHT dht(DHTPIN, DHTTYPE);

#define MAC_TEMP_ALLOWED 30.0



bool heating = false;
bool tooHot = false;
String currentHeatingEventID = "";

float readScale(){
  //wait for the scale to be readt
  while(!h.is_ready()) {
    //Serial.print("Weight sensed is ");
    //Serial.println(weight);
  }
  float value = h.get_units();
  // Serial.print(value);
  // Serial.print(" ");
  // Serial.println(heating);
  return value;
}

void checkTooHot(){
  float t = dht.readTemperature();
  if(isnan(t)){
    return;
  }

  tooHot = t > MAC_TEMP_ALLOWED;
}

void setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(500000);
  Serial1.begin(500000);
   while (!Serial) {
     // wait for serial port to connect.
   }
  matrix.begin();
  while(!Serial1){

  }
  //Serial.write((char)0);
  //Serial.println("Hello World");
  connectToWIFI();// defined in cloudConnection.h
  
  //setup and zero the sacle
  h.begin(scaleDataPin, scaleClockPin);
  h.tare();
  h.set_scale(10000);
  dht.begin();

  pinMode(REPLAY_PIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Ready");
}

void checkCatHeating(){
//when currently heating

    //check the sensor value to see if there might not be a cat there anymore
    float weight = readScale();
    //if the sclae reading is mroe the the weight it should turn off at then keep it on
    if(weight > MAX_WEIGHT){
      Serial.println("High Weight");
      heating = false;
      Serial.println("Turning Off");
    }
    if(weight < MIN_WEIGHT){
      Serial.println("Low Weight");
      heating = false;
      Serial.println("Turning Off");
    }

    //take a picture with the camera
    bool isCat=false;
    //send the picture to the could for processing
    HTTPResponse catImageresponse = setialInputPostRequest("/isCat");//read image / send image to the cloud


    if(responseOK(&catImageresponse)){
       isCat = catImageresponse.content[0] == '1';
     }else{
       //perhaps stop heating here just to be safe
       heating = false;
       Serial.print("Cloud returned an error: ");
       Serial.println(catImageresponse.status);
       Serial.println(catImageresponse.content);
       //if the cloud resturned an error then return
       return;
     }
    

    //if there is not a cat presenet:
    if(!isCat){

      //turn the heating off


      heating = false;

      
    }

    if(!heating)
    {
      //tell the cloud that heating has stopped using the heating event ID from before
      HTTPResponse heatingStartResponse = basicGetRequest("/heatingStopped?id="+currentHeatingEventID);
      if(responseOK(&heatingStartResponse)){
        //evenrthiong is fine, not need to do much
      }else{
        Serial.print("Cloud returned an error: ");
        Serial.println(heatingStartResponse.status);
        Serial.println(heatingStartResponse.content);
        //if the cloud returned an error then return
        return;
      }
    }
}

void checkCatNotHeating(){
//when not heating

    //check the value of the weight sensor
    //if the value might indicate a cat:
    float weight = readScale();
    //if the sclae reading is more the the weight it should turn on at then turn it on
    if(weight < MIN_WEIGHT){
      Serial.println("Low Weight");
      return;
    }
    else if(weight > MAX_WEIGHT)
    {
      Serial.println("High Weight");
      return;
    }
    
    //take a picture with the camera
    bool isCat=true;
    //send the picture to the could for processing
     HTTPResponse catImageresponse = setialInputPostRequest("/isCat");//read image / send image to the cloud
    
    // //check the response status
     if(responseOK(&catImageresponse)){
       isCat = catImageresponse.content[0] == '1';
     }else{
        Serial.print("Cloud returned an error: ");
        Serial.println(catImageresponse.status);
        Serial.println(catImageresponse.content);
       //if the cloud resturned an error then return
       return;
     }
    //if it is a cat:
    if(isCat){

      //turn the heating on
      Serial.println("Cat detected. Turning On Heat");

      //tell the cloud heating has started and recieve the heating event ID
      HTTPResponse heatingStartResponse = basicGetRequest("/heatingStarted");
      if(responseOK(&heatingStartResponse)){
        currentHeatingEventID = heatingStartResponse.content;

        Serial.println(heatingStartResponse.status);
        Serial.println(heatingStartResponse.contentLength);
        Serial.println(heatingStartResponse.contentType);
        Serial.println(heatingStartResponse.contentEncoding);
        Serial.println(heatingStartResponse.content);
      }else{
         Serial.print("Cloud returned an error: ");
         Serial.println(heatingStartResponse.status);
         Serial.println(heatingStartResponse.content);
        //if the cloud returned an error then return
        return;
      }

      heating = true;
    }
}

void loop(){
  //cat detecttion things
  if(heating){
    checkCatHeating();
  }else{
    checkCatNotHeating();
  }

  //wait a momnent before checking again
  delay(200);

  //heating controlll thigns prbly
  checkTooHot();

  digitalWrite(REPLAY_PIN, heating && !tooHot);
  digitalWrite(LED_BUILTIN, heating && !tooHot);
  // while(1){
  //   delay(5000);
  // }
}
