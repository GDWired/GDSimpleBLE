/*
	The service GDNativeBLE is accessible at this address: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
	This service has 3 characteristics:
		- 1 RX in WRITE mode: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E
		- 2 TX in NOTIFY mode: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E
		- 3 TX in READ mode: 6E400004-B5A3-F393-E0A9-E50E24DCCA9E

	If you send data to the first one, the rxValueCount value will be incremented, and your data will be sent to the serial @115200 baud.
	If you subscribe to the second one, every second the value txValue will be incremented and sent.
	If you read the data on the last one, you will get the rxValueCount value.
*/

#include <Arduino.h> // If you are in ArduinoIDE remove it
#include <BLEDevice.h>
#include <BLE2902.h>

// Server BLE data
BLEServer* server = nullptr;
BLEService* service = nullptr;
BLECharacteristic* rxCharacteristic = nullptr;
BLECharacteristic* txCharacteristic = nullptr;
BLECharacteristic* txCharacteristicReadOnly = nullptr;

// Internal data
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
uint8_t rxValueCount = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID                        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
#define CHARACTERISTIC_UUID_RX              "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX              "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_READ_ONLY_UUID_TX    "6E400004-B5A3-F393-E0A9-E50E24DCCA9E"

// The server callbacks (used to detect new connection/disconnection)
class ServerCallbacks: public BLEServerCallbacks {
	// Connected
	void onConnect(BLEServer* pServer) {
	  deviceConnected = true;
	}
	// Disconnected
	void onDisconnect(BLEServer* pServer) {
	  deviceConnected = false;
	}
};

// Callbacks used to received data from the client (write)
class CharacteristicCallbacks: public BLECharacteristicCallbacks {
	// On write (from client)
	void onWrite(BLECharacteristic *characteristic) {
		std::string rxValue = characteristic->getValue();

		// Just print data in serial
		if (rxValue.length() > 0) {
			Serial.print("Received Value: ");
			for (int i = 0; i < rxValue.length(); i++) {
				Serial.print(rxValue[i]);
			}
			Serial.println();
		}
		rxValueCount++;

		// Set new value in the READ characteristic
		String message = "Data received count: " + String(rxValueCount);
		txCharacteristicReadOnly->setValue(message.c_str());
	}
};

// Arduino setup function
void setup() {
	// To write data from the client
	Serial.begin(115200);

	// Create the BLE Device
	BLEDevice::init("GDNativeBLE");

	// Create the BLE Server
	server = BLEDevice::createServer();
	server->setCallbacks(new ServerCallbacks());

	// Create the BLE Service
	service = server->createService(SERVICE_UUID);

	// Create a BLE Characteristic (NOTIFY)
	txCharacteristic = service->createCharacteristic( CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
	txCharacteristic->addDescriptor(new BLE2902());

	// Create a BLE Characteristic (READ)
	txCharacteristicReadOnly = service->createCharacteristic(CHARACTERISTIC_READ_ONLY_UUID_TX, BLECharacteristic::PROPERTY_READ);
	txCharacteristicReadOnly->addDescriptor(new BLE2902());

	// Create a BLE Characteristic (WRITE)
	rxCharacteristic = service->createCharacteristic( CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
	rxCharacteristic->setCallbacks(new CharacteristicCallbacks());

	// Start the service
	service->start();

	// Start advertising
	server->getAdvertising()->start();
	Serial.println("Start advertising");
}

// Arduino main loop
void loop() {

	if (deviceConnected) {
		// Notify data
		txCharacteristic->setValue(String(txValue).c_str());
		txCharacteristic->notify();

		txValue++;
		delay(1000); // One packet per seconds
	}

	// Disconnecting
	if (!deviceConnected && oldDeviceConnected) {
		delay(500); // Give the bluetooth stack the chance to get things ready
		server->startAdvertising();
		Serial.println("Restart advertising");
		oldDeviceConnected = deviceConnected;
	}

	// Connecting
	if (deviceConnected && !oldDeviceConnected) {
		oldDeviceConnected = deviceConnected;
	}
}
