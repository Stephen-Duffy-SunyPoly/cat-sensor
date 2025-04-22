
#include "cloudConnection.h"


void setup(){
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
  }

  connectToWIFI();// defined in cloudConnection.h
  //do any other required setup here

  //test to se what a repsonse looks like
  Serial.println("Attempting HTTP Request");
  //exampleHTTPResuest("/");
  HTTPResponse response = basicGetRequest("/");
  Serial.print("Code: ");
  Serial.println(response.status);
  Serial.print("length: ");
  Serial.println(response.contentLength);
  Serial.print("content: ");
  Serial.println(response.content);

  Serial.println("RESUEST COMPLETED!");

}

void loop(){

}
