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

extern uint16_t hid_conn_id;
extern bool sec_conn;

xQueueHandle keyboard_queue = NULL;
xQueueHandle consumer_queue = NULL;

static uint8_t myssid[] = "ROOT", mypassword[] = "root201314";

void post_item(uint8_t *key_value)
{
    ESP_LOGD(TAG, "the key vaule = %d, %d, %d, %d, %d, %d, %d, %d", key_value[0],
             key_value[1], key_value[2], key_value[3], key_value[4], key_value[5], key_value[6], key_value[7]);
    extern xQueueHandle keyboard_queue;
    xQueueSendToBack(keyboard_queue, key_value, 0);
    memset(key_value, 0, HID_KEYBOARD_IN_RPT_LEN);
    xQueueSendToBack(keyboard_queue, key_value, 0);
}

static void keyboard_send_task(void *pvParameters)
{
    uint8_t key_value[8];
    for (;;) {
        if (xQueueReceive(keyboard_queue, &key_value, portMAX_DELAY)) {
            esp_hidd_send_keyboard_value(hid_conn_id, key_value);
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
    }
}

static void consumer_send_task(void *pvParameters)
{
    uint8_t consumer_vaule[2];
    for (;;) {
        if (xQueueReceive(consumer_queue, &consumer_vaule, portMAX_DELAY)) {
            esp_hidd_send_consumer_value(hid_conn_id, consumer_vaule[0], consumer_vaule[1]);
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
    }
}

/* Function to initialize SPIFFS */
static esp_err_t init_spiffs(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 50,   // 这决定了可在存储上创建的最大文件数
        .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    return ESP_OK;
}

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

    /* Initialize file storage */
    ESP_ERROR_CHECK(init_spiffs());

    wifi_connect(FAST_SCAN, myssid, mypassword);

    /* Start the file server */
    configure_server("/spiffs");

    start_ble_hid_server();

    ESP_ERROR_CHECK(configure_key_scan_array());

    //keyboard queue special key(1U)|number key(1U)|key vaule(6U)
    keyboard_queue = xQueueCreate(1024, HID_KEYBOARD_IN_RPT_LEN);

    //consumer queue Consumer key(1U)|key pressed(1U)
    consumer_queue = xQueueCreate(10, 2U);

    xTaskCreatePinnedToCore(keyboard_send_task, "keyboard_send_task", 2048, NULL, 10, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(consumer_send_task, "consumer_send_task", 2048, NULL, 5, NULL, tskNO_AFFINITY);
    //xTaskCreatePinnedToCore(keyboard_scan, "keyboard_scan", 2048, NULL, 0, NULL, tskNO_AFFINITY);

}
