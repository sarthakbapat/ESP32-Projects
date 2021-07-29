#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define LED_BUILTIN   14

BLECharacteristic *pCharacteristic;
BLEServer *pServer;
bool deviceConnected = false;

#define SERVICE_UUID              "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX    "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect (BLEServer *pServer) {
    deviceConnected = true;
  }

  void onDisconnect (BLEServer *pServer) {
    deviceConnected = false;
  }
  
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {

    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      Serial.println("*****************");
      Serial.println("New Value Received: ");
      for(int i=0; i<value.length(); i++){
        Serial.print(value[i]);
      }
    }
    if (value == "ON") {
      Serial.println("Turning ON the Led");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (value == "OFF") {
      Serial.println("Turning off the Led");
      digitalWrite(LED_BUILTIN, LOW);
    }
    
  }
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Create the BLE device
  BLEDevice::init("ESP32");

  // Create the BLE server
 // BLEServer *pServer = BLEDevice::createServer();
  pServer = BLEDevice::createServer();

  // Create the BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);  

  // Create a BLE characteristic.
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks());
 
  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for client connection to notify .....");

}

void loop() {
  // put your main code here, to run repeatedly:

  if (deviceConnected) {

    delay(2000);
  }

}
