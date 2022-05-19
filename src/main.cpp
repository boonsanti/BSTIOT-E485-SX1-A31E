#include <WiFi.h>           // WiFi control for ESP32

#include <ModbusMaster.h>

#define RXD2 16
#define TXD2 17

// instantiate ModbusMaster object
ModbusMaster PowerMeter;

bool state = true;

void readPowerParameters() {
  uint8_t j, result1, result2;
  uint16_t data;

  float gridVoltage;
  float gridFrequency;
  float gridCurrent;
  uint32_t gridActivePower;
  uint32_t gridActiveEnergy;

  float solarVoltage;
  float solarFrequency;
  float solarCurrent;
  uint32_t solarActivePower;
  uint32_t solarActiveEnergy;

//  freeHeap["n"] = "free_heap";
//  freeHeap["u"] = "byte";
//  freeHeap["v"] = ESP.getFreeHeap();
//  parameters.add(freeHeap);
//
//  wifiRSSI["n"] = "wifi_rssi";
//  wifiRSSI["u"] = "dBm";
//  wifiRSSI["v"] = WiFi.RSSI();
//  parameters.add(wifiRSSI);

  // slave: read (6) 16-bit registers starting at register 0x66 to RX buffer
  result1 = PowerMeter.readHoldingRegisters(0x66, 14);

  // do something with data if read is successful
  if (result1 == PowerMeter.ku8MBSuccess)
  { 
    // Read Line Voltage (RMS) 0x66
    data = PowerMeter.getResponseBuffer(0);
    gridVoltage = data / 100.0;
    Serial.printf("Voltage : %0.2f V ", gridVoltage);

    // Read Frequency 0x69
    data = PowerMeter.getResponseBuffer(3);
    gridFrequency = data / 10.0;
    Serial.printf(" , Frequency : %0.1f Hz ", gridFrequency);

    // Read Line Current (RMS) 0x70
    data = PowerMeter.getResponseBuffer(10);
    gridCurrent = data / 100.0;
    Serial.printf(" , Current : %0.2f A ", gridCurrent);

    // Read Active Power (W) 0x73
    gridActivePower = PowerMeter.getResponseBuffer(13);
    Serial.printf(" , ActivePower : %d W ", gridActivePower);

    // Read Active Energy (Wh) 0x6E
    gridActiveEnergy = PowerMeter.getResponseBuffer(8);
    gridActiveEnergy = gridActiveEnergy << 16;
    gridActiveEnergy |= PowerMeter.getResponseBuffer(9);
    Serial.printf(" , ActiveEnergy : %d Wh \r\n", gridActiveEnergy);

  } else {
    Serial.println("Cannot read power meter data.");
  }
}

void setup()
{

  // Modbus communication runs at 115200 baud
  Serial.begin(115200);

  Serial2.begin(1200, SERIAL_8E1, RXD2, TXD2);
  // Modbus slave ID 10
  PowerMeter.begin(10, Serial2);

}

void loop()
{
  readPowerParameters();

  delay(2000);
}
