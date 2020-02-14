#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <drivers/gpio.h>

typedef struct {
  const char *dev_name;
  gpio_pin_t pin;
  gpio_flags_t flags;
  gpio_callback_handler_t callback_handler;
  struct gpio_callback callback;
  struct device *dev;
} button_t;

int button_init(button_t *button);

#endif
