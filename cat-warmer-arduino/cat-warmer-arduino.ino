
#include "cloudConnection.h"

#define IMAGE_DATA_LENGTH 320*240*2

bool heating = false;
String currentHeatingEventID = "";

void setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  //Serial.begin(500000);
  Serial1.begin(500000);
  // while (!Serial) {
  //   // wait for serial port to connect. Needed for communicating woth camera
  // }
  while(!Serial1){

  }
  //Serial.write((char)0);
  //Serial.println("Hello World");
  connectToWIFI();// defined in cloudConnection.h
  //do any other required setup here
  digitalWrite(LED_BUILTIN, HIGH);
  basicGetRequest("/connectedToWifi");
  basicGetRequest("/connectedToWifi2");
}

void checkCatHeating(){
// //when currently heating

    // //check the sensor value to see if there might not be a cat there anymore

    // //take a picture with the camera 
    // char imageData[IMAGE_DATA_LENGTH];
    // takePicture(imageData);
    // bool isCat = false;
    // //send the picture to the could for processing
    // HTTPResponse catImageresponse = basicPostRequest("/isCat",imageData,IMAGE_DATA_LENGTH);
    // if(responseOK(&catImageresponse)){
    //   isCat = catImageresponse.content[0] == '1';
    // }else{
    //   //perhaps stop heating here just to be safe
    //   // Serial.print("Cloud returned an error: ");
    //   // Serial.println(catImageresponse.status);
    //   // Serial.println(catImageresponse.content);
    //   //if the cloud resturned an error then return
    //   return;
    // }
    

    // //if there is not a cat presenet:
    // if(!isCat){

    //   //turn the heating off

    //   //tell the cloud that heating has stopped using the heating event ID from before
    //   HTTPResponse heatingStartResponse = basicGetRequest("/heatingStopped?id="+currentHeatingEventID);
    //   if(responseOK(&heatingStartResponse)){
    //     //evenrthiong is fine, not need to do much
    //   }else{
    //     // Serial.print("Cloud returned an error: ");
    //     // Serial.println(heatingStartResponse.status);
    //     // Serial.println(heatingStartResponse.content);
    //     //if the cloud returned an error then return
    //     return;
    //   }

    //   heating = false;
    // }
}

void checkCatNotHeating(){
//when not heating

    //check the value of the weight sensor
    //if the value might indicate a cat:
    basicGetRequest("/NH1");
    //take a picture with the camera
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    basicGetRequest("/NH2");
    bool isCat=true;;
    digitalWrite(LED_BUILTIN, HIGH);
    basicGetRequest("/NH3");
    //send the picture to the could for processing
    Serial1.write((char)1);
    HTTPResponse catImageresponse = setialInputPostRequest("/isCat",IMAGE_DATA_LENGTH);
    basicGetRequest("/NH4");
    digitalWrite(LED_BUILTIN, LOW);
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
  //Serial.println("Doign request");
  basicGetRequest("/looop");
  digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  //cat detecttion things
  if(heating){
    checkCatHeating();
  }else{
    checkCatNotHeating();
  }

  //wait a momnent before checking again

  //heating controlll thigns prbly
}
