#ifndef __LED_H__
#define __LED_H__

#include <drivers/gpio.h>

typedef struct {
  const char *dev_name;
  gpio_pin_t pin;
  struct device *dev;
} led_t;

int led_init(led_t *led);
void led_set(led_t *led, int value);
void led_blink(led_t *led);

#endif
