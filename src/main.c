#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <logging/log.h>

#include "led.h"
#include "button.h"
#include "ble.h"

#include <devicetree.h>
#include <drivers/sensor.h>

#define BME280 DT_INST(0, bosch_bme280)

#if DT_HAS_NODE(BME280)
#define BME280_LABEL DT_LABEL(BME280)
#else
#error Your devicetree has no enabled nodes with compatible "bosch,bme280"
#define BME280_LABEL "<none>"
#endif

LOG_MODULE_REGISTER(main);

void main(void) {
  LOG_INF("starting...");

  struct device *dev = device_get_binding(BME280_LABEL);
  if (dev == NULL) {
    LOG_ERR("could not find BME280 device");
    return;
  }

  ble_init();

  while (1) {
    struct sensor_value value;
    sensor_sample_fetch(dev);
    sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &value);

    // value.val1 -> integer part
    // value.val2 -> fractional part (* 10e-6)
    // degrees_c = value.val1 + value.val2 * 10^(-6)

    // temp = degrees * 100
    s16_t temp = (value.val1 * 100) + (value.val2 / 10000);
    LOG_INF("temp: %d", temp);

    ble_notify((u8_t *) &temp, 2); // s16_t -> 2 bytes

    k_sleep(K_MSEC(5000));
  }
}
