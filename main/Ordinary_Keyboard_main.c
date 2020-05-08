#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "esp_event.h"
#include "esp_spiffs.h"
#include "esp_netif.h"

#include "esp_hidd_prf_api.h"
#include "Ordinary_Keyboard_main.h"

#define TAG "main"

void app_main(void)
{
    esp_err_t ret;

    // Initialize NVS.
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    start_ble_hid_server();

    ESP_ERROR_CHECK(configure_key_scan_array());

    xTaskCreatePinnedToCore(keyboard_scan, "keyboard_scan", 2048, NULL, 0, NULL, tskNO_AFFINITY);

}
