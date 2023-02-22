#include <global.h>

bool write_led = false;

void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value = pCharacteristic->getValue();

        if (value.length() > 0)
        {
            Serial.println("*********");
            Serial.print("New value: ");
            for (int i = 0; i < value.length(); i++)
            {
                Serial.print(value[i]);
            }

            Serial.println();
            Serial.println("*********");
        }
    }
