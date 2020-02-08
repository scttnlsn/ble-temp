#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>

#include "led.h"
#include "button.h"

static int button_state = 0;

void button_pressed(struct device *dev, struct gpio_callback *cb, u32_t pins) {
  button_state = !button_state;
}

void main(void) {
  int ret;

  led_t led;
  led.dev_name = DT_ALIAS_LED0_GPIOS_CONTROLLER;
  led.pin = DT_ALIAS_LED0_GPIOS_PIN;

  ret = led_init(&led);
  if (ret < 0) {
    return;
  }

  button_t button;
  button.dev_name = DT_GPIO_KEYS_BUTTON_0_GPIOS_CONTROLLER;
  button.pin = DT_GPIO_KEYS_BUTTON_0_GPIOS_PIN;
  button.flags = DT_GPIO_KEYS_BUTTON_0_GPIOS_FLAGS;
  button.callback_handler = button_pressed;

  ret = button_init(&button);
  if (ret < 0) {
    return;
  }

  while (1) {
    led_set(&led, button_state);
  }
}
