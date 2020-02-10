#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>

#include "led.h"
#include "button.h"

static struct gpio_callback button_callback;

K_FIFO_DEFINE(button_presses_fifo);

typedef struct {
  void *fifo_reserved;
} button_press_event_t;

void button_pressed(struct device *dev, struct gpio_callback *cb, u32_t pins) {
  button_press_event_t event;
  k_fifo_put(&button_presses_fifo, &event);
}

void main(void) {
  printk("starting...\n");

  int ret;

  led_t led;
  led.dev_name = "GPIO_0";
  led.pin = 18;

  ret = led_init(&led);
  if (ret < 0) {
    printk("error initializing LED\n");
    return;
  }

  button_t button;
  button.dev_name = "GPIO_0";
  button.pin = 16;
  button.flags = GPIO_PULL_UP | GPIO_ACTIVE_LOW;
  button.callback = button_callback;
  button.callback_handler = button_pressed;

  ret = button_init(&button);
  if (ret < 0) {
    printk("error initializing button\n");
    return;
  }

  int button_state = 0;

  while (1) {
    button_press_event_t *event = k_fifo_get(&button_presses_fifo, K_FOREVER);
    button_state = !button_state;
    printk("press\n");
    led_set(&led, button_state);
  }
}
