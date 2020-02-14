#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <sys/types.h>
#include <logging/log.h>
#include <zephyr.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include "ble.h"

LOG_MODULE_REGISTER(ble);

static u8_t val = 0;

// 8D9D7800-5B61-412A-AB71-5C7E0E559086
static struct bt_uuid_128 uuid_service = BT_UUID_INIT_128(
  0x86, 0x90, 0x55, 0x0E, 0x7E, 0x5C, 0x71, 0xAB,
  0x2A, 0x41, 0x61, 0x5B, 0x00, 0x78, 0x9D, 0x8D);

// 8D9D7801-5B61-412A-AB71-5C7E0E559086
static struct bt_uuid_128 uuid_characteristic = BT_UUID_INIT_128(
  0x86, 0x90, 0x55, 0x0E, 0x7E, 0x5C, 0x71, 0xAB,
  0x2A, 0x41, 0x61, 0x5B, 0x01, 0x78, 0x9D, 0x8D);

static ssize_t ble_read_value(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset) {
  const uint8_t *value = attr->user_data;
  return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(*value));
}

static void ble_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value) {
  ARG_UNUSED(attr);

  bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
  LOG_INF("notifications: %s", notif_enabled ? "enabled" : "disabled");
}

BT_GATT_SERVICE_DEFINE(ble_gatt_service,
  BT_GATT_PRIMARY_SERVICE(&uuid_service),

  BT_GATT_CHARACTERISTIC(
    &uuid_characteristic.uuid,
    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
    BT_GATT_PERM_READ,
    ble_read_value,
    NULL,
    &val),

  BT_GATT_CCC(ble_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

// advertisement data
static const struct bt_data ad[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR))
};

// scan response data
static const struct bt_data sd[] = {
  BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, (sizeof(CONFIG_BT_DEVICE_NAME) - 1)),
};

static void ble_connected(struct bt_conn *conn, u8_t err) {
  if (err) {
    LOG_ERR("connection failed: %u", err);
  } else {
    LOG_INF("connected");
  }
}

static void ble_disconnected(struct bt_conn *conn, u8_t reason) {
  LOG_INF("disconnected: %u", reason);
}

static struct bt_conn_cb conn_callbacks = {
  .connected = ble_connected,
  .disconnected = ble_disconnected,
};

void ble_ready(int err) {
  if (err) {
    LOG_ERR("enable failed: %d", err);
    return;
  }

  LOG_INF("ready");

  err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
  if (err) {
    LOG_ERR("advertising error: %d", err);
    return;
  }

  LOG_INF("advertising started");
}

void ble_init(void) {
  int err = bt_enable(ble_ready);
  if (err) {
    LOG_ERR("failed to enable: %d)", err);
    return;
  }

  bt_conn_cb_register(&conn_callbacks);
}

void ble_notify(uint8_t *data, int length) {
  bt_gatt_notify(NULL, &ble_gatt_service.attrs[1], data, length);
}
