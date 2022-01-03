#include <Arduino.h>
#include "algorithm.h"
#include "max30102.h"
#include <Adafruit_BMP280.h>

#define MAX_BRIGHTNESS 255
#if defined(ARDUINO_AVR_UNO)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated.  Samples become 16-bit data.
uint16_t aun_ir_buffer[100]; //infrared LED sensor data
uint16_t aun_red_buffer[100];  //red LED sensor data
#else
uint32_t aun_ir_buffer[100]; //infrared LED sensor data
uint32_t aun_red_buffer[100];  //red LED sensor data
#endif
int32_t n_ir_buffer_length; //data length
int32_t n_spo2;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;

Adafruit_BMP280 bmp; // I2C Interface
void setup() {

  maxim_max30102_reset(); //resets the MAX30102
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  pinMode(10, INPUT);  //pin D10 connects to the interrupt output pin of the MAX30102
  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_dummy); //Reads/clears the interrupt status register
  maxim_max30102_init();  //initialize the MAX30102
  bmp.begin();
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                  uch_dummy=Serial.read();
}

// the loop routine runs over and over again forever:
void loop() {
  uint32_t un_min, un_max, un_prev_data, un_brightness;  //variables to calculate the on-board LED brightness that reflects the heartbeats
  int32_t i;
  float f_temp;

  un_brightness = 0;
  un_min = 0x3FFFF;
  un_max = 0;

  n_ir_buffer_length = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  for (i = 0; i < n_ir_buffer_length; i++)
  {
    //    while(digitalRead(10)==1);  //wait until the interrupt pin asserts
    maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i)); //read from MAX30102 FIFO

    if (un_min > aun_red_buffer[i])
      un_min = aun_red_buffer[i]; //update signal min
    if (un_max < aun_red_buffer[i])
      un_max = aun_red_buffer[i]; //update signal max
    Serial.print(F("red="));
    Serial.print(aun_red_buffer[i], DEC);
    Serial.print(F(", ir="));
    Serial.println(aun_ir_buffer[i], DEC);
  }
  un_prev_data = aun_red_buffer[i];
  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    i = 0;
    un_min = 0x3FFFF;
    un_max = 0;

    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (i = 25; i < 100; i++)
    {
      aun_red_buffer[i - 25] = aun_red_buffer[i];
      aun_ir_buffer[i - 25] = aun_ir_buffer[i];

      //update the signal min and max
      if (un_min > aun_red_buffer[i])
        un_min = aun_red_buffer[i];
      if (un_max < aun_red_buffer[i])
        un_max = aun_red_buffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (i = 75; i < 100; i++)
    {
      un_prev_data = aun_red_buffer[i - 1];
      //      while(digitalRead(10)==1);
      //      digitalWrite(9, !digitalRead(9));
      maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i));

      //calculate the brightness of the LED
      if (aun_red_buffer[i] > un_prev_data)
      {
        f_temp = aun_red_buffer[i] - un_prev_data;
        f_temp /= (un_max - un_min);
        f_temp *= MAX_BRIGHTNESS;
        f_temp = un_brightness - f_temp;
        if (f_temp < 0)
          un_brightness = 0;
        else
          un_brightness = (int)f_temp;
      }
      else
      {
        f_temp = un_prev_data - aun_red_buffer[i];
        f_temp /= (un_max - un_min);
        f_temp *= MAX_BRIGHTNESS;
        un_brightness += (int)f_temp;
        if (un_brightness > MAX_BRIGHTNESS)
          un_brightness = MAX_BRIGHTNESS;
      }
    }

    //send samples and calculation result to terminal program through UART
    if (ch_hr_valid == 1) {
      Serial.print(n_heart_rate, DEC);
    }
    Serial.print(bmp.readAltitude(1019.66));
    Serial.print("m");
    Serial.println();
  }
  maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
}
