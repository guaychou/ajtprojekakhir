#include "BLEDevice.h" 
#include <WiFi.h> 
#include <PubSubClient.h> 
#include <HTTPClient.h>
#define wifi_ssid "Realme3" 
#define wifi_password "kocakkocak456" 
#define token "http://168.63.242.174/webservice.php"
WiFiClient espClient; 
PubSubClient client(espClient); 
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"); 
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"); 
static boolean doConnect = false; 
static boolean connected = false; 
static boolean doScan = false; 
static BLERemoteCharacteristic* pRemoteCharacteristic; 
static BLEAdvertisedDevice* myDevice; 
String pData;

class MyClientCallback : public BLEClientCallbacks { 
  void onConnect(BLEClient* pclient) { 
    Serial.print ("lagi konek"); 
  } 

  void onDisconnect(BLEClient* pclient) { 
    connected = false; 
    Serial.println("onDisconnect"); 
  } 
}; 

bool connectToServer() { 
  Serial.print("Forming a connection to "); 
  Serial.println(myDevice->getAddress().toString().c_str()); 

  BLEClient* pClient = BLEDevice::createClient(); 
  Serial.println(" - Created client"); 

  pClient->setClientCallbacks(new MyClientCallback()); 
  pClient->connect(myDevice); 
  Serial.println(" - Connected to server"); 
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID); 
  if (pRemoteService == nullptr) { 
    pClient->disconnect(); 
    return false; 
  } 
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID); 
  
  if (pRemoteCharacteristic == nullptr) { 
  pClient->disconnect(); 
  return false; 
  } 

  if(pRemoteCharacteristic->canRead()) { 
  std::string value = pRemoteCharacteristic->readValue(); 
  Serial.println(value.c_str()); 
  } 

  if(pRemoteCharacteristic->canNotify()) 
  pRemoteCharacteristic->registerForNotify(notifyCallback); 

  connected = true; 
  } 
  void notifyCallback( 
BLERemoteCharacteristic* pBLERemoteCharacteristic, 
uint8_t* pDataT, 
size_t length, 
bool isNotify  ) { 
  int httpResponseCode;
  String response;
  HTTPClient http;
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str()); 
  Serial.println(length); 
  Serial.print("data: "); 
  //Serial.println((char*) pDataT); 
  char *pDataX;
  pData = (String)(char*) pDataT;
  Serial.println(pData);
  
    //delay(1000); 
  if (doConnect == true) { 
      http.begin(token);  //Specify destination for HTTP request
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");             //Specify content-type header
      int httpResponseCode = http.POST(pData);
      //Send the actual POST request
      Serial.println("Lagi ngepos hehe :)");
      if(httpResponseCode>0){
      response = http.getString();
      Serial.print("Kode Respon : ");
      Serial.println(httpResponseCode);
      Serial.println(response);                      //Get the response to the request
      }else {
        Serial.println("Something wrong with you");
        }  
    } else { 
      Serial.println("FAIL"); 
    }
    http.end(); 
    delay(10000);
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks { 
  void onResult(BLEAdvertisedDevice advertisedDevice) { 
    Serial.print("BLE Advertised Device found: "); 
    Serial.println(advertisedDevice.toString().c_str()); 
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) { 
      BLEDevice::getScan()->stop(); 
      myDevice = new BLEAdvertisedDevice(advertisedDevice); 
      doConnect = true; 
      doScan = true; 
    } 
  } 
};
void setup() { 
  Serial.begin(115200); 
  BLEDevice::init(""); 
  BLEScan* pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); 
  pBLEScan->setInterval(1349); 
  pBLEScan->setWindow(449); 
  pBLEScan->setActiveScan(true); 
  pBLEScan->start(5, false); 
  setup_wifi(); 
  client.setServer(thingsboardServer, 1883); 
} 

void setup_wifi() { 
  delay(20); 
  WiFi.begin(wifi_ssid, wifi_password); 
  Serial.println("WiFi is OK "); 
}
 
void reconnect() { 
  while (!client.connected()) { 
    WiFi.begin(wifi_ssid, wifi_password); 
    if ( client.connect("ESP32 Device", token, NULL) ) { 
    Serial.println( "[DONE]" ); 
    } else { 
    Serial.print( "[FAILED]" ); 
    } 
  } 
} 
void loop() { 
  if (connected) { 
    String newValue = "Time since boot: " + String(millis()/1000); 
    Serial.println("Setting new characteristic value to \"" + newValue + "\""); 
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length()); 
  }else if(doScan){ 
    BLEDevice::getScan()->start(0); 
  } 
  connectToServer();
  //delay(1000); 
}
