#pragma once

#include "driver/i2c.h"
#include "esp_check.h"
#include "esp_log.h"

#define I2C_MASTER_TX_BUF_DISABLE 0                /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                /*!< I2C master do not need buffer */
#define I2C_MASTER_TX_BUF_DISABLE 0                /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                /*!< I2C master doesn't need buffer */
#define ACK_CHECK_EN              0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS             0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                   0x0              /*!< I2C ack value */
#define NACK_VAL                  0x1              /*!< I2C nack value */
#define WRITE_BIT                 I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                  I2C_MASTER_READ  /*!< I2C master read */

typedef uint8_t device_address_t;
typedef uint8_t i2c_reg_t;
typedef bool    i2c_busy_t;

esp_err_t i2c_init(void);
esp_err_t i2c_write_byte(device_address_t addr, i2c_reg_t reg, uint8_t* data);
esp_err_t i2c_read_u32(device_address_t addr, i2c_reg_t reg, uint32_t* data);
esp_err_t i2c_read_u16(device_address_t addr, i2c_reg_t reg, uint16_t* data);
esp_err_t i2c_write_block(device_address_t addr, i2c_reg_t reg, uint8_t* data, size_t len);
esp_err_t i2c_read_byte(device_address_t addr, i2c_reg_t reg, uint8_t* data);
esp_err_t i2c_read_u32(device_address_t addr, i2c_reg_t reg, uint32_t* data);
esp_err_t i2c_read_u16(device_address_t addr, i2c_reg_t reg, uint16_t* data);
esp_err_t i2c_read_block(device_address_t addr, i2c_reg_t reg, uint8_t* data, size_t len);
esp_err_t i2c_read_block_no_reg(device_address_t addr, uint8_t* data, size_t len);