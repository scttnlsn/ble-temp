# Read BMS status notifications sent via BLE
#
# Usage:
#   python ble.py "<MAC ADDR>"
#

import bluepy
import struct
import sys
import uuid

mac = sys.argv[1]
characteristic_uuid = uuid.UUID('8D9D7801-5B61-412A-AB71-5C7E0E559086')

class NotificationDelegate(bluepy.btle.DefaultDelegate):

    def __init__(self):
        bluepy.btle.DefaultDelegate.__init__(self)

    def handleNotification(self, handle, data):
        val = struct.unpack('h', data)
        print(val)

print('connecting to %s...' % mac)
peripheral = bluepy.btle.Peripheral(mac, bluepy.btle.ADDR_TYPE_RANDOM)
peripheral.setDelegate(NotificationDelegate())

try:
    characteristic = peripheral.getCharacteristics(uuid=characteristic_uuid)[0]

    print('listening...')
    notify_handle = characteristic.getHandle() + 1
    peripheral.writeCharacteristic(notify_handle, b'\x01\x00', withResponse=True)

    while True:
        if peripheral.waitForNotifications(5.0):
            continue
finally:
    print('disconnect')
    peripheral.disconnect()
