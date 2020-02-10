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
                           GPIO_INPUT | button->flags);
  if (ret < 0) {
    return -1;
  }

  if (button->callback_handler != NULL) {
    ret = gpio_pin_interrupt_configure(button->dev,
                                       button->pin,
                                       GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);
    if (ret < 0) {
      return -1;
    }

    gpio_init_callback(&button->callback,
                       button->callback_handler,
                       BIT(button->pin));

    gpio_add_callback(button->dev, &button->callback);
  }

  return 0;
}
