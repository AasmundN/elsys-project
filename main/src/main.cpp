// libraries
#include <Arduino.h>

// custom files
#include <setup.h>
#include <global.h>

TaskHandle_t Task2;

i2s_config_t i2s_config = {
   .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
   .sample_rate = I2S_SAMPLE_RATE,
   .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
   .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
   .communication_format = I2S_COMM_FORMAT_I2S_LSB,
   .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
   .dma_buf_count = 2,
   .dma_buf_len = 1024,
   .use_apll = false,
   .tx_desc_auto_clear = false,
   .fixed_mclk = 0
};

// void setup
void setup() {
   // setup Serial
   Serial.begin(115200); 
   
   Serial.println();
   Serial.println("------------------------------------");
   Serial.println("-          Starting setup          -");
   Serial.println("------------------------------------");
   Serial.println();

   // create adcSampler instance
   Serial.print("Setup running on core ");
   Serial.println(xPortGetCoreID());

   adcSampler = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7, i2s_config);
   adcSampler->start();

   // setup functions
   setupMatrix();
   setupBluetooth();
   setupTasks(Task2);

   // setup microphone pin
   pinMode(AUDIO_IN_PIN, INPUT);

   // setup built in LED
   pinMode(LED, OUTPUT);
   digitalWrite(LED, LOW);

   Serial.println();
   Serial.println("------------------------------------");
   Serial.println("-            Setup done            -");
   Serial.println("------------------------------------");
   Serial.println();
}

void loop() {
   
}
