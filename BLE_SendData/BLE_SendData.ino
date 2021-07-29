#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
BLECharacteristic *pCharacteristic_2;
bool deviceConnected = false;
int txValue = 0;
int txValue_2 = 0;

#define SERVICE_UUID              "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX    "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX_2    "6E400005-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect (BLEServer *pServer) {
    deviceConnected = true;
  }

  void onDisconnect (BLEServer *pServer) {
    deviceConnected = false;
  }
  
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Create the BLE device
  BLEDevice::init("ESP32");

  // Create the BLE server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);  

  // Create a BLE characteristic.
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic_2 = pService->createCharacteristic(CHARACTERISTIC_UUID_TX_2, BLECharacteristic::PROPERTY_NOTIFY);

  // BLE2902 needed to notify.
  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic_2->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for clinet connection to notify .....");

  

}

void loop() {
  // put your main code here, to run repeatedly:

  if (deviceConnected) {
    txValue = random(-10, 20);
    txValue_2 = random(70, 90);

    // Conversion of txValue
    char txString[8];
    dtostrf(txValue, 1, 2, txString);

    // Conversion of txValue_2
    char txString2[8];
    dtostrf(txValue_2, 1, 2, txString2);

    // Setting the value to the characteristic
    pCharacteristic->setValue(txString);
    pCharacteristic_2->setValue(txString2);

    // Notifying the connected client.
    pCharacteristic->notify();
    pCharacteristic_2->notify();
    Serial.println("Sent value: " + String(txString));
    Serial.println("Sent value_2: " + String(txString2));

    delay(1000);
  }

}
