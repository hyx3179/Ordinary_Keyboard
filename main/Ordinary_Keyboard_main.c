#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"

#include "esp_hidd_prf_api.h"
#include "keyboard_macro.h"

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

esp_err_t start_ble_hid_server();

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

    //keyboard queue special key(1U)|number key(1U)|key vaule(6U)
    keyboard_queue = xQueueCreate(10, 8U);

    //consumer queue Consumer key(1U)|key pressed(1U)
    consumer_queue = xQueueCreate(10, 2U);

    xTaskCreate(keyboard_send_task, "keyboard_send_task", 2048, NULL, 5, NULL);
    xTaskCreate(consumer_send_task, "consumer_send_task", 2048, NULL, 10, NULL);

    //text
    printf_hello_world();
}
