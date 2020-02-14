#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <logging/log.h>

#include "led.h"
#include "button.h"
#include "ble.h"

LOG_MODULE_REGISTER(main);

static struct gpio_callback button_callback;

K_FIFO_DEFINE(button_presses_fifo);

typedef struct {
  void *fifo_reserved;
} button_press_event_t;

static s64_t last_press = 0;

void button_pressed(struct device *dev, struct gpio_callback *cb, u32_t pins) {
  s64_t current_press = k_uptime_get();

  // debounce
  if (current_press - last_press > 200) {
    button_press_event_t event;
    k_fifo_put(&button_presses_fifo, &event);
  }

  last_press = current_press;
}

void main(void) {
  LOG_INF("starting...");

  int err;

  led_t led;
  led.dev_name = "GPIO_0";
  led.pin = 18;

  err = led_init(&led);
  if (err) {
    printk("error initializing LED\n");
    return;
  }

  button_t button;
  button.dev_name = "GPIO_0";
  button.pin = 16;
  button.flags = GPIO_PULL_UP | GPIO_ACTIVE_LOW;
  button.callback = button_callback;
  button.callback_handler = button_pressed;

  err = button_init(&button);
  if (err) {
    LOG_ERR("error initializing button");
    return;
  }

  ble_init();

  int button_state = 0;
  u8_t counter = 0;

  while (1) {
    button_press_event_t *event = k_fifo_get(&button_presses_fifo, K_FOREVER);
    button_state = !button_state;
    LOG_INF("button pressed");
    led_set(&led, button_state);
    ble_notify((u8_t *) &counter, 1);
    counter++;
  }
}
