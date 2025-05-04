
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

#define scaleDataPin 7
#define scaleClockPin 8
HX711 h;

#define MIN_ON_WEIGHT 10.0
#define MIN_OFF_WEIGHT 9.0





bool heating = false;
String currentHeatingEventID = "";

float readScale(){
  //wait for the scale to be readt
  while(!h.is_ready()) {
    //Serial.print("Weight sensed is ");
    //Serial.println(weight);
  }
  return h.get_value();
}

void setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(500000);
  Serial1.begin(500000);
  // while (!Serial) {
  //   // wait for serial port to connect. Needed for communicating woth camera
  // }
  matrix.begin();
  while(!Serial1){

  }
  //Serial.write((char)0);
  //Serial.println("Hello World");
  connectToWIFI();// defined in cloudConnection.h
  
  //setup and zero the sacle
  h.begin(scaleDataPin, scaleClockPin);
  h.tare();

  digitalWrite(LED_BUILTIN, HIGH);
}

void checkCatHeating(){
//when currently heating

    //check the sensor value to see if there might not be a cat there anymore
    float weight = readScale();
    //if the sclae reading is mroe the the weight it should turn off at then keep it on
    if(weight > MIN_OFF_WEIGHT){
      return;
    }

    //take a picture with the camera
    bool isCat=true;;
    //send the picture to the could for processing
    HTTPResponse catImageresponse = setialInputPostRequest("/isCat",IMAGE_DATA_LENGTH, NUMBER_OF_IMAGE_LINES);//read image / send image to the cloud


    if(responseOK(&catImageresponse)){
      isCat = catImageresponse.content[0] == '1';
    }else{
      //perhaps stop heating here just to be safe
      // Serial.print("Cloud returned an error: ");
      // Serial.println(catImageresponse.status);
      // Serial.println(catImageresponse.content);
      //if the cloud resturned an error then return
      return;
    }
    

    //if there is not a cat presenet:
    if(!isCat){

      //turn the heating off

      //tell the cloud that heating has stopped using the heating event ID from before
      HTTPResponse heatingStartResponse = basicGetRequest("/heatingStopped?id="+currentHeatingEventID);
      if(responseOK(&heatingStartResponse)){
        //evenrthiong is fine, not need to do much
      }else{
        // Serial.print("Cloud returned an error: ");
        // Serial.println(heatingStartResponse.status);
        // Serial.println(heatingStartResponse.content);
        //if the cloud returned an error then return
        return;
      }

      heating = false;
    }
}

void checkCatNotHeating(){
//when not heating

    //check the value of the weight sensor
    //if the value might indicate a cat:
    float weight = readScale();
    //if the sclae reading is more the the weight it should turn on at then turn it on
    if(weight > MIN_ON_WEIGHT){
      return;
    }
    
    //take a picture with the camera
    bool isCat=true;;
    //send the picture to the could for processing
    HTTPResponse catImageresponse = setialInputPostRequest("/isCat",IMAGE_DATA_LENGTH, NUMBER_OF_IMAGE_LINES);//read image / send image to the cloud
    
    //check the response status
    if(responseOK(&catImageresponse)){
      isCat = catImageresponse.content[0] == '1';
    }else{
      // Serial.print("Cloud returned an error: ");
      // Serial.println(catImageresponse.status);
      // Serial.println(catImageresponse.content);
      //if the cloud resturned an error then return
      return;
    }
    //if it is a cat:
    if(isCat){

      //turn the heating on

      //tell the cloud heating has started and recieve the heating event ID
      HTTPResponse heatingStartResponse = basicGetRequest("/heatingStarted");
      if(responseOK(&heatingStartResponse)){
        currentHeatingEventID = heatingStartResponse.content;
      }else{
        // Serial.print("Cloud returned an error: ");
        // Serial.println(heatingStartResponse.status);
        // Serial.println(heatingStartResponse.content);
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

  //heating controlll thigns prbly
  while(1){
    delay(5000);
  }
}
