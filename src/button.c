#include "button.h"

int button_init(button_t *button) {
  int ret;

  struct device *dev = device_get_binding(button->dev_name);
  if (dev == NULL) {
    return -1;
  }

  button->dev = dev;

  ret = gpio_pin_configure(button->dev,
                           button->pin,
                           button->flags | GPIO_INPUT);
  if (ret < 0) {
    return -1;
  }

  ret = gpio_pin_interrupt_configure(button->dev,
                                     button->pin,
                                     GPIO_INT_EDGE_TO_ACTIVE);
  if (ret < 0) {
    return -1;
  }

  struct gpio_callback callback;
  button->callback = callback;

  gpio_init_callback(&callback,
                     button->callback_handler,
                     BIT(button->pin));

  gpio_add_callback(button->dev, &callback);

  return 0;
}
