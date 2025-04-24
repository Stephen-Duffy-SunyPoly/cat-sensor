
#include "cloudConnection.h"

bool heating = false;
String currentHeatingEventID = "";


void setup(){
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
  }

  connectToWIFI();// defined in cloudConnection.h
  //do any other required setup here

  //test to se what a repsonse looks like
  // Serial.println("Attempting HTTP Request");
  // //exampleHTTPResuest("/");
  // HTTPResponse response = basicGetRequest("/");
  // Serial.print("Code: ");
  // Serial.println(response.status);
  // Serial.print("length: ");
  // Serial.println(response.contentLength);
  // Serial.print("content: ");
  // Serial.println(response.content);

  // Serial.println("RESUEST COMPLETED!");

}

void loop(){
  //cat detecttion things
  if(heating){
    //when currently heating

    //check the sensor value to see if there might not be a cat there anymore

    //take a picture with the camera 
    char imageData[];
    int imageDataLength;
    bool isCat;
    //send the picture to the could for processing
    HTTPResponse catImageresponse = basicPostRequest("/isCat",imageData,imageDataLength);
    if(responseOK(catImageresponse)){
      isCat = catImageresponse.content[0] == '1';
    }else{
      //perhaps stop heating here just to be safe
      Serial.print("Cloud returned an error: ");
      Serial.println(catImageresponse.status);
      Serial.println(catImageresponse.content);
      //if the cloud resturned an error then return
      return;
    }
    

    //if there is not a cat presenet:
    if(!isCat){

      //turn the heating off

      //tell the cloud that heating has stopped using the heating event ID from before
      HTTPResponse heatingStartResponse = basicGetRequest("/heatingStopped?id="+currentHeatingEventID);
      if(responseOK(heatingStartResponse)){
        //evenrthiong is fine, not need to do much
      }else{
        Serial.print("Cloud returned an error: ");
        Serial.println(heatingStartResponse.status);
        Serial.println(heatingStartResponse.content);
        //if the cloud returned an error then return
        return;
      }

      heating = false;
    }

     
  }else{
    //when not heating

    //check the value of the weight sensor
    //if the value might indicate a cat:

    //take a picture with the camera

    char imageData[];
    int imageDataLength;
    bool isCat;
    //send the picture to the could for processing
    HTTPResponse catImageresponse = basicPostRequest("/isCat",imageData,imageDataLength);
    if(responseOK(catImageresponse)){
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

      //tell the cloud heating has started and recieve the heating event ID
      HTTPResponse heatingStartResponse = basicGetRequest("/heatingStarted");
      if(responseOK(heatingStartResponse)){
        currentHeatingEventID = heatingStartResponse.content;
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

  //wait a momnent before checking again

  //heating controlll thigns prbly
}
