#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "DHTesp.h"
DHTesp dht;
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};
void setup() {
  dht.setup(27, DHTesp::DHT11);
  Serial.begin(115200);
  BLEDevice::init("ESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  String payload = "";
    if (deviceConnected) {
    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();
   payload = "suhu=";payload += temperature;payload += "&";
   payload += "kelembapan="; payload += humidity;
    char attributes[100];
    payload.toCharArray( attributes, 100 );
    pCharacteristic->setValue(attributes);
    pCharacteristic->notify();
        delay(20000);
    }
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); 
        pServer->startAdvertising(); 
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    if (deviceConnected && !oldDeviceConnected) {
        Serial.println ("Lagi Konek");
        oldDeviceConnected = deviceConnected;
        Serial.println (payload);
    }
}
