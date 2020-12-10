# nrf-ble

## Setup

```
pip install west
west update
source env.sh
```

## Build

**NRF52840 DK**

```
west build -p auto -b nrf52840dk_nrf52840
west flash
```

**NRF52840 Dongle**

* https://docs.zephyrproject.org/latest/boards/arm/nrf52840dongle_nrf52840/doc/index.html
* https://infocenter.nordicsemi.com/pdf/nRF52840_Dongle_User_Guide_v1.1.pdf
  - section 6.3.2 is about how to power from external regulated source like a battery

```
west build -p auto -b nrf52840dongle_nrf52840
nrfutil pkg generate --hw-version 52 --sd-req=0x00 --application build/zephyr/zephyr.hex --application-version 1 firmware.zip
nrfutil dfu usb-serial -pkg firmware.zip -p /dev/ttyACM0
```

## Config

To access the interactive Kconfig menu:

```
west build -t menuconfig
```

## Resources

* https://devzone.nordicsemi.com/nordic/nordic-blog/b/blog/posts/optimizing-power-on-nrf52-designs
* https://devzone.nordicsemi.com/nordic/short-range-guides/b/hardware-and-layout/posts/current-measurement-guide-introduction
* https://github.com/joric/bluetosis
* https://github.com/jpconstantineau/BlueMicro_BLE

Modules:

* EByte e73
* Fanstel BT840
