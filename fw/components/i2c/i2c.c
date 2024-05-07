#include "i2c.h"

const gpio_num_t i2c_gpio_sda  = GPIO_NUM_36;
const gpio_num_t i2c_gpio_scl  = GPIO_NUM_35;
const i2c_port_t i2c_port      = I2C_NUM_0;
const uint32_t   i2c_frequency = 100000;

static const char* TAG         = "I2C";

/**
 * @brief Initialises I2C interface
 * @return ESP_OK   Initialisation success
 */
esp_err_t i2c_init(void)
{
    esp_err_t ret     = ESP_OK;

    i2c_config_t conf = {
        .mode             = I2C_MODE_MASTER,
        .sda_io_num       = i2c_gpio_sda,
        .sda_pullup_en    = GPIO_PULLUP_DISABLE,
        .scl_io_num       = i2c_gpio_scl,
        .scl_pullup_en    = GPIO_PULLUP_DISABLE,
        .master.clk_speed = i2c_frequency,
    };

    ret = i2c_param_config(i2c_port, &conf);
    ESP_ERROR_CHECK(ret);
    ESP_LOGD(TAG, "Driver parameter configuration is successful.");

    ret = i2c_driver_install(i2c_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "Driver has been installed successfully.");

    return ret;
}

/**
 * @brief Initiate a write command of a single byte
 *
 * Wrapper for i2c_write_block()
 *
 * @param addr      slave address
 * @param reg       destination register
 * @param data      data to be written
 *
 * @return ESP_OK   success
 */
esp_err_t i2c_write_byte(device_address_t addr, i2c_reg_t reg, uint8_t* data)
{
    esp_err_t ret = ESP_OK;

    ret           = i2c_write_block(addr, reg, data, 1);

    return ret;
}

/**
 * @brief Initiate a write command of 'len' bytes
 *
 * @param addr      slave address
 * @param reg       destination register
 * @param data      data to be written
 * @param len       length of data to be written
 *
 * @return ESP_OK   success
 */
esp_err_t i2c_write_block(device_address_t addr, i2c_reg_t reg, uint8_t* data, size_t len)
{
    esp_err_t        ret = ESP_OK;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    ret                  = i2c_master_start(cmd);
    ret                  = i2c_master_write_byte(cmd, addr << 1 | WRITE_BIT, ACK_CHECK_EN);
    ret                  = i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);

    for(int i = 0; i < len; i++)
    {
        i2c_master_write_byte(cmd, data[i], ACK_CHECK_EN);
    }
    ret = i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

/**
 * @brief Initiate a read command of a single byte
 *
 * Wrapper for i2c_read_block()
 *
 * @param addr      slave address
 * @param reg       register
 * @param data      pointer for destination data
 *
 * @return ESP_OK   success
 */
esp_err_t i2c_read_byte(device_address_t addr, i2c_reg_t reg, uint8_t* data)
{
    esp_err_t ret = ESP_OK;

    ret           = i2c_read_block(addr, reg, data, 1);

    return ret;
}

/**
 * @brief Initiate a read command of 'len' bytes
 *
 * @param addr      slave address
 * @param reg       register
 * @param data      pointer for destination data
 * @param len       length of data to be read
 *
 * @return ESP_OK   success
 */
esp_err_t i2c_read_block(device_address_t addr, i2c_reg_t reg, uint8_t* data, size_t len)
{
    esp_err_t        ret = ESP_OK;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    ret                  = i2c_master_start(cmd);
    ret                  = i2c_master_write_byte(cmd, addr << 1 | WRITE_BIT, ACK_CHECK_EN);
    ret                  = i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
    ret                  = i2c_master_start(cmd);
    ret                  = i2c_master_write_byte(cmd, addr << 1 | READ_BIT, ACK_CHECK_EN);

    for(size_t i = 0; i < len; i++)
    {
        if(i2c_master_read_byte(cmd, (data + i), ((i == len - 1) ? NACK_VAL : ACK_VAL)) != ESP_OK)
        {
            break;
        }
    }

    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

/**
 * @brief Initiate a read command of 'len' bytes
 *
 * @param addr      slave address
 * @param reg       register
 * @param data      pointer for destination data
 * @param len       length of data to be read
 *
 * @return ESP_OK   success
 */
esp_err_t i2c_read_u32(device_address_t addr, i2c_reg_t reg, uint32_t* data)
{
    esp_err_t        ret = ESP_OK;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    ret                  = i2c_master_start(cmd);
    ret                  = i2c_master_write_byte(cmd, addr << 1 | WRITE_BIT, ACK_CHECK_EN);
    ret                  = i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
    ret                  = i2c_master_start(cmd);
    ret                  = i2c_master_write_byte(cmd, addr << 1 | READ_BIT, ACK_CHECK_EN);

    for(size_t i = 0; i < 4; i++)
    {
        if(i2c_master_read_byte(cmd, ((uint8_t*)data + i), ((i == 4 - 1) ? NACK_VAL : ACK_VAL)) != ESP_OK)
        {
            break;
        }
    }

    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

/**
 * @brief Initiate a read command of 'len' bytes
 *
 * @param addr      slave address
 * @param reg       register
 * @param data      pointer for destination data
 * @param len       length of data to be read
 *
 * @return ESP_OK   success
 */
esp_err_t i2c_read_u16(device_address_t addr, i2c_reg_t reg, uint16_t* data)
{
    esp_err_t        ret = ESP_OK;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    ret                  = i2c_master_start(cmd);
    ret                  = i2c_master_write_byte(cmd, addr << 1 | WRITE_BIT, ACK_CHECK_EN);
    ret                  = i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
    ret                  = i2c_master_start(cmd);
    ret                  = i2c_master_write_byte(cmd, addr << 1 | READ_BIT, ACK_CHECK_EN);

    for(size_t i = 0; i < 2; i++)
    {
        if(i2c_master_read_byte(cmd, ((uint8_t*)data + i), ((i == 2 - 1) ? NACK_VAL : ACK_VAL)) != ESP_OK)
        {
            break;
        }
    }

    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

/**
 * @brief Initiate a read command of 'len' bytes but no data register
 *
 * Special function to be used for SHT4x type sensors as they do not implement
 * I2C communication the way other sensors do.
 *
 * @param addr      slave address
 * @param data      pointer for destination data
 * @param len       length of data to be read
 *
 * @return ESP_OK   success
 */
esp_err_t i2c_read_block_no_reg(device_address_t addr, uint8_t* data, size_t len)
{
    esp_err_t        ret = ESP_OK;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    ret                  = i2c_master_start(cmd);
    ret                  = i2c_master_write_byte(cmd, addr << 1 | READ_BIT, ACK_CHECK_EN);

    for(size_t i = 0; i < len; i++)
    {
        i2c_master_read_byte(cmd, data + i, ACK_VAL);
    }

    ret = i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}
