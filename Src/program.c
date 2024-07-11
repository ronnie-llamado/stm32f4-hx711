
#include "dio.h"
#include "exti.h"
#include "hx711.h"
#include "led.h"
#include "maf.h"
#include "main.h"
#include "program.h"
#include "timer.h"

#define MS_TIMER TIM10_BASE
#define US_TIMER TIM11_BASE
#define LED_PORT GPIOA_BASE
#define LED_PIN 5UL

#define HX711_DATA_PORT GPIOA_BASE
#define HX711_DATA_PIN 8UL
#define HX711_CLOCK_PORT GPIOB_BASE
#define HX711_CLOCK_PIN 5UL

#define HX711_DATA_EXTI_PORT 1
#define HX711_DATA_EXTI_PIN HX711_DATA_PIN

#define NEOPIXEL_DOUT_PORT GPIOA_BASE
#define NEOPIXEL_DOUT_PIN 9UL

// Timers
struct timer_t * ms_timer;
struct timer_t * us_timer;

// DIO
struct dio_t * led_dio;
struct dio_t * hx711_clock;
struct dio_t * hx711_data;

// EXTI
struct exti_t * hx711_data_is_ready_exti;

// HX711
struct hx711_t * hx711;

// LED
struct led_t * led;

// MAF
struct maf_t * maf;

bool hx711_data_is_ready;

void tare(void);

void setup(void)
{
  us_timer = timer_init(US_TIMER);
  timer_set_prescaler(us_timer, 0);
  timer_set_period(us_timer, 84);
  timer_enable(us_timer);

  ms_timer = timer_init(MS_TIMER);
  timer_set_prescaler(ms_timer, 84000);
  timer_set_period(ms_timer, 1000);
  timer_enable(ms_timer);

  led_dio = dio_init(LED_PORT, LED_PIN, DIO_DIRECTION_OUTPUT);

  hx711_data = dio_init(HX711_DATA_PORT, HX711_DATA_PIN, DIO_DIRECITON_INPUT);
  hx711_clock = dio_init(HX711_CLOCK_PORT, HX711_CLOCK_PIN, DIO_DIRECTION_OUTPUT);
  hx711 = hx711_init(hx711_data, hx711_clock, us_timer);

  hx711_data_is_ready_exti = exti_init(HX711_DATA_EXTI_PORT, HX711_DATA_EXTI_PIN, EXTI_EDGE_FALLING);

  led = led_init(led_dio, ms_timer);

  maf = maf_init();
}

int32_t raw = 0;
int32_t data = 0;
int32_t offset = 0;
int32_t calibrated = 0;
float relative = 0;
float voltage = 0;
float kilogram = 0;
float pound = 0;

int32_t prev_data = 0;
bool stable = false;

#define STABLE_THRESHOLD 64
#define STABLE_MAX 128
#define OUT_OF_RANGE_THRESHOLD 256
#define OUT_OF_RANGE_MAX 1024

void loop(void)
{
    uint32_t stable_count = 0;
    uint32_t out_of_range_count = OUT_OF_RANGE_MAX;

    for(;;)
    {
        led_process(led);

        if (true == hx711_data_is_ready)
        {
          hx711_data_is_ready = false;

          //disable
          exti_disable(hx711_data_is_ready_exti);

          raw = hx711_read_count(hx711); 

          maf_add(maf, raw);
          data = maf_get_average(maf);

          calibrated = data - offset;

          relative = calibrated / (float)((1 << 24) - 1.0f);
          // 5V / 128 (HX711 Gain) 
          // voltage = relative * (5.0f / 128.0f);
          voltage = relative * 0.0390625f;
          // Load cell conversion (200kg/10mV) @ 5V excitation
          // kilogram = voltage * (200.0f / 0.01f);
          kilogram = voltage * 20000.0f;
          pound = kilogram * 2.20462262185f;

          // Auto-tare functionality
          // Check if data deviates from previous offset and is stable.
          // If both are true, set a new offset.
          if (abs(data - offset) > OUT_OF_RANGE_THRESHOLD)
          {
            if (out_of_range_count < OUT_OF_RANGE_MAX) out_of_range_count++;
          } else {
            out_of_range_count = 0;
          }

          if (abs(data - prev_data) < STABLE_THRESHOLD) 
          {
            if (stable_count < STABLE_MAX) stable_count++;
          } else {
            stable_count = 0;
          }

          if ((stable_count == STABLE_MAX) && (out_of_range_count == OUT_OF_RANGE_MAX))
          {
            offset = data;
            out_of_range_count = 0;
            stable_count = 0;
          }

          prev_data = data;

          // enable
          exti_enable(hx711_data_is_ready_exti);
        }
    }
}

void tare(void)
{

}