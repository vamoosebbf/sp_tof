#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#define BOARD_MAIX_CUBE     1
#define BOARD_MAIX_NEW_GO   2

#define BOARD BOARD_MAIX_CUBE
#define BOARD_NAME  "MAIX_CUBE"
#define TAG "VL53L0X"
#if (BOARD == BOARD_MAIX_CUBE)

#define VL53L0X_I2C_DEVICE 0
#define VL53L0X_I2C_FREQ_KHZ 100
#define VL53L0X_SCL 6
#define VL53L0X_SDA 7
#define VL53L0X_SHT 8

#endif

#endif
