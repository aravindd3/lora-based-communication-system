#include <driver/i2s.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 26
#define BUTTON_PIN 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SAMPLE_RATE 16000
#define BUFFER_LEN 256

int16_t buffer[BUFFER_LEN];
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = BUFFER_LEN,
    .use_apll = false
  };
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void loop() {
  bool recording = (digitalRead(BUTTON_PIN) == LOW);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  if (recording) {
    display.print("Recording");
    size_t bytes_read;
    i2s_read(I2S_NUM_0, buffer, sizeof(buffer), &bytes_read, portMAX_DELAY);
    display.drawLine(0, 32, 127, 32, SSD1306_WHITE);
    for (int x = 0; x < 128; x++) {
      int index = x * (BUFFER_LEN / 128);
      int16_t sample = buffer[index] / 500;
      int y = 32 + sample;
      if (y < 0) y = 0;
      if (y > 63) y = 63;
      display.drawPixel(x, y, SSD1306_WHITE);
    }
  } else {
    display.print("Stopped");
  }
  display.display();
}
