/*
MIT License

Copyright (c) 2025
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include <log.hpp>

SerialDebug mySerial(115200L);

class BatteryVoltage {
 private:
  float _batteryLevel = 0;

 public:
  BatteryVoltage();
  void read();
  float getVoltage() { return _batteryLevel; }
};

class Config {
 public:
  int getVoltagePin() { 
#if defined(ESP8266)
    return PIN_A0; 
#elif defined(ESP32C3)
    return A3; 
#elif defined(ESP32S2)
    return A2; 
#elif defined(ESP32S3)
    return A1; 
#elif defined(ESP32)
    return PIN_A0; 
#else
  #error "Unknown platform"
#endif
  }
  float getVoltageFactor() { 
#if defined(ESP8266)
    return 1.59;  // 220k to Batt+
#elif defined(ESP32C3)
    return 2.55;  // 220k + 220k
#elif defined(ESP32S2)
    return 2.45; // 220k + 220k
#elif defined(ESP32S3)
    return 3.05;  // 220k + 220k
#elif defined(ESP32LITE)
    return 2.45; 
#elif defined(ESP32)
    return 2.45; 
#else
  #error "Unknown platform"
#endif
  }
};

BatteryVoltage myBatteryVoltage;
Config myConfig;

void setup() {
  delay(1000);
}

void loop() {
  myBatteryVoltage.read();
  delay(2000);
}

BatteryVoltage::BatteryVoltage() {
#if defined(ESP8266)
  pinMode(myConfig.getVoltagePin(), INPUT);
#else
  pinMode(myConfig.getVoltagePin(), INPUT);
  analogReadResolution(SOC_ADC_MAX_BITWIDTH);
  // Max input values per board (2.5V is the a good setting)
  // ESP32: 2450mV
  // ESP32-S2: 2500mV
  // ESP32-S3: 3100mV
  // ESP32-C3: 2500mV
  // ESP32-C6: 3300mV  
  analogSetAttenuation(ADC_11db); 
#endif
}

void BatteryVoltage::read() {
  float factor = myConfig.getVoltageFactor(); 
  int v = analogRead(myConfig.getVoltagePin());

#if defined(ESP8266)
  _batteryLevel = ((3.3 / 1023) * v) * factor;
#elif defined(ESP32S2)
  _batteryLevel = ((2.5 / ((1 << SOC_ADC_MAX_BITWIDTH)-1)) * v) * factor;
#elif defined(ESP32S3)
  _batteryLevel = ((3.1 / ((1 << SOC_ADC_MAX_BITWIDTH)-1)) * v) * factor;
#elif defined(ESP32C3)
  _batteryLevel = ((2.5 / ((1 << SOC_ADC_MAX_BITWIDTH)-1)) * v) * factor;
#elif defined(ESP32LITE)
  _batteryLevel = ((2.4 / ((1 << SOC_ADC_MAX_BITWIDTH)-1)) * v) * factor;
#elif defined(ESP32)
  _batteryLevel = ((2.4 / ((1 << SOC_ADC_MAX_BITWIDTH)-1)) * v) * factor;
#endif

  Log.verbose(
      F("BATT: Reading voltage level. Factor=%F Value=%d, Voltage=%F." CR),
      factor, v, _batteryLevel);
}

// EOF
