#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include <sys/param.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_spiffs.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "esp_hidd_prf_api.h"
#include "keyboard_macro.h"

#define TAG "example"

extern uint16_t hid_conn_id;
extern bool sec_conn;

xQueueHandle keyboard_queue = NULL;
xQueueHandle consumer_queue = NULL;

static void keyboard_send_task(void *pvParameters)
{
    uint8_t key_vaule[8];
    for (;;) {
        if (xQueueReceive(keyboard_queue, &key_vaule, portMAX_DELAY)) {
            esp_hidd_send_keyboard_value(hid_conn_id, key_vaule);
        }
    }
}

static void consumer_send_task(void *pvParameters)
{
    uint8_t consumer_vaule[2];
    for (;;) {
        if (xQueueReceive(consumer_queue, &consumer_vaule, portMAX_DELAY)) {
            esp_hidd_send_consumer_value(hid_conn_id, consumer_vaule[0], consumer_vaule[1]);
        }
    }
}

void printf_hello_world()
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    while (1) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        if (sec_conn) {
            uint8_t str[] = "\\STHello World!\\EN/";
            keyboard_macro_handle(str, sizeof(str));
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
        .max_files = 50,   // This decides the maximum number of files that can be created on the storage
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

esp_err_t start_ble_hid_server();
esp_err_t configure_server(const char *base_path);

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

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    /* Initialize file storage */
    ESP_ERROR_CHECK(init_spiffs());

    /* Start the file server */
    configure_server("/spiffs");


    start_ble_hid_server();

    //keyboard queue special key(1U)|number key(1U)|key vaule(6U)
    keyboard_queue = xQueueCreate(10, 8U);

    //consumer queue Consumer key(1U)|key pressed(1U)
    consumer_queue = xQueueCreate(10, 2U);

    xTaskCreate(keyboard_send_task, "keyboard_send_task", 2048, NULL, 5, NULL);
    xTaskCreate(consumer_send_task, "consumer_send_task", 2048, NULL, 10, NULL);

    //text
    //printf_hello_world();
}
