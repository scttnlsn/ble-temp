# nrf-ble

Setup:

```
git clone https://github.com/zephyrproject-rtos/zephyr.git
pip install west
west update
source env.sh
```

Build:

```
mkdir build
cd build
BOARD=nrf51_ble400 cmake ..
make
```

Flash:

```
openocd -f interface/stlink-v2.cfg -f target/nrf51.cfg -c "program ./zephyr/zephyr.bin verify reset exit"
```
