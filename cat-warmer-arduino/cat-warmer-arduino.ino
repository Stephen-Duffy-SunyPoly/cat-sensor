#include <WiFiS3.h>


//WARNING NEVER COMMIT ACTUAL WIFI CREDENTIALS
const char WIFI_NAME[] = "WIFI NAME HERE";
const char WIFI_PASSWORD[] = "WIFI PASSWORD HERE";
int status = WL_IDLE_STATUS;


const String CLOUD_ADDRESS = "192.168.0.0";//make sure to set this to the correct ip for the cloud device
const uint16_t CLOUD_PORT = 8080;

void setup(){
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true){
      delay(1000);
    }
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_NAME);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the data:
  Serial.println("Connected to the network");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  //do any other required setup here

}

void loop(){

}

/**
@param requestPath the path and query string part of the url for the request. must begin with a /
note this will not work with https connections
*/
void exampleHTTPResuest(String requestPath){
  WiFiClient client;//HTTP client for interacting with the cloud
  //make the connection
  if (client.connect(CLOUD_ADDRESS.c_str(), CLOUD_PORT)) {
    // manually set the http headers:
    client.println("GET "+requestPath+" HTTP/1.1");//set request method, path, and protocall
    //client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }else{
    Serial.println("Error: failed to connect to cloud");
    return;
  }

  //read the response
  //at this time I am not entirly sure how this works
  //I think that the read method will end up reaturning everything starting with the http response headers 
  //if that is the case then we will need to put in some work to poase those
  //otherwise I have no idea how we will get the status code 
  uint32_t received_data_num = 0;
  while (client.available()) {
    /* actual data reception */
    char c = client.read();
    /* print data to serial port */
    Serial.print(c);
    /* wrap data to 80 columns*/
    received_data_num++;
  }  
  client.stop();
}