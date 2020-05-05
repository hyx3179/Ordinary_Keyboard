#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "hid_dev.h"
#include "Ordinary_Keyboard_main.h"

#define TAG "keyboard_scan"

//static bool configure_statu = false;

static uint8_t horizontal_scan_gpio_list[] = {
    GPIO_NUM_19,
    GPIO_NUM_5,
};

static uint8_t vertical_scan_gpio_list[] = {
    GPIO_NUM_18,
    GPIO_NUM_17,
};

static uint64_t get_gpio_bit_mask(uint8_t *gpio_list, int gpio_num)
{
    uint64_t pin_bit_mask = 0;
    for (int i = 0; i < gpio_num; i++) {
        pin_bit_mask += 1ULL << gpio_list[i];
    }
    return pin_bit_mask;
}

key_property[sizeof(vertical_scan_gpio_list)][sizeof(vertical_scan_gpio_list)] = {
    false, HID_KEY_A, 'a', 0,
    false, HID_KEY_B, 'b', 0,
    false, HID_KEY_C, 'c', 0,
    false, HID_KEY_D, 'd', 0,
};

esp_err_t configure_key_scan_array()
{
    esp_err_t ret;

    gpio_config_t horizontal_scan_gpio_config = {
        .pin_bit_mask = get_gpio_bit_mask(horizontal_scan_gpio_list, key_property),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ret = gpio_config(&horizontal_scan_gpio_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Configure horizontal scan gpio Error!");
        return ret;
    }

    gpio_config_t vertical_scan_gpio_config = {
        .pin_bit_mask = get_gpio_bit_mask(vertical_scan_gpio_list, sizeof(vertical_scan_gpio_list)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ret = gpio_config(&vertical_scan_gpio_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Configure vertical scan gpio Error!");
        return ret;
    }

    return ret;
}

void keyboard_scan()
{
    for (;;) {
        for (int vertical = 0; vertical < sizeof(vertical_scan_gpio_list); vertical++) {
            gpio_set_level(vertical_scan_gpio_list[vertical], true);
            for (int horizontal = 0; horizontal < sizeof(horizontal_scan_gpio_list); horizontal++) {
                if (gpio_get_level(horizontal_scan_gpio_list[horizontal])) {
                    ESP_LOGI(TAG, "axaj : vertical %d horizontal %d", vertical, horizontal);
                }
            }
            gpio_set_level(vertical_scan_gpio_list[vertical], false);
        }
    }
}