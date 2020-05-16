#include <drivers/gpio.h>

#include "led.h"

int led_init(led_t *led) {
  struct device *dev = device_get_binding(led->dev_name);
  if (dev == NULL) {
    return -1;
  }

  led->dev = dev;

  int ret = gpio_pin_configure(led->dev, led->pin, GPIO_OUTPUT_ACTIVE);
  if (ret < 0) {
    return -1;
  }

  return 0;
}

void led_set(led_t *led, int value) {
  gpio_pin_set(led->dev, led->pin, value);
}

void led_blink(led_t *led) {
  while (1) {
    led_set(led, 1);
    k_sleep(K_MSEC(100));
    led_set(led, 0);
    k_sleep(K_MSEC(100));
  }
}
