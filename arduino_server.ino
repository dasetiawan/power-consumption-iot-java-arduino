#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 256;
int idx = 0;
float dataset[] = {
  159, 151, 148, 146, 147, 151, 159, 169, 174, 182, 188, 189, 187, 185, 182, 180, 183, 194, 196, 186, 178, 171, 163,
  171, 164, 161, 160, 163, 170, 183, 198, 203, 207, 211, 209, 205, 201, 197, 193, 194, 209, 216, 211, 206, 198, 186,
  180, 173, 169, 168, 170, 178, 193, 206, 210, 213, 213, 214, 210, 206, 201, 198, 203, 219, 226, 222, 216, 208, 197,
  201, 197, 195, 194, 197, 206, 220, 235, 237, 238, 239, 237, 231, 227, 223, 221, 222, 236, 242, 238, 232, 223, 208,
  197, 193, 193, 194, 198, 206, 219, 233, 239, 245, 247, 241, 233, 228, 223, 221, 225, 242, 254, 252, 249, 241, 229,
  185, 179, 176, 173, 174, 177, 184, 193, 199, 205, 207, 206, 203, 199, 194, 193, 199, 219, 235, 235, 233, 226, 218,
  221, 215, 211, 210, 210, 214, 218, 226, 232, 236, 236, 229, 217, 209, 201, 199, 201, 213, 220, 218, 218, 213, 206    
};
 
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

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
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    if (deviceConnected) {
        if (idx == 168) {
          idx = 0;
        }
        value = dataset[idx];
        pCharacteristic->setValue((uint8_t*)&value, 4);
        pCharacteristic->notify();
        idx++;
        delay(2000); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
