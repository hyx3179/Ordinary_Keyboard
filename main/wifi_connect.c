

#include <string.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "Ordinary_Keyboard_main.h"

#define TAG "wifi_connect"

#define DEFAULT_RSSI -127
static bool netif_initialized = false;

void scan(int scan_list_size)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    uint16_t number = scan_list_size;
    wifi_ap_record_t ap_info[scan_list_size];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
    for (int i = 0; (i < scan_list_size) && (i < ap_count); i++) {
        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
    }
}

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}

/* Initialize Wi-Fi as sta and set scan method */
esp_err_t fast_scan(uint8_t *myssid, uint8_t *mypassword)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));

    // Initialize and start WiFi
    wifi_config_t wifi_config = {
        .sta = {
            .scan_method = WIFI_FAST_SCAN,
            .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
            .threshold.rssi = DEFAULT_RSSI,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    memcpy(&wifi_config.sta.ssid, myssid, sizeof(wifi_config.sta.ssid));
    memcpy(&wifi_config.sta.password, mypassword, sizeof(wifi_config.sta.password));

    ESP_LOGI(TAG, "SSID %s", wifi_config.sta.ssid);
    ESP_LOGI(TAG, "PASSWORD %s", wifi_config.sta.password);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    return ESP_OK;
}

esp_err_t wifi_connect(int scan_mode, uint8_t *myssid, uint8_t *mypassword)
{
    int scan_list_size = 8;
    if (!netif_initialized) {
        ESP_ERROR_CHECK(esp_netif_init());
        ESP_ERROR_CHECK(esp_event_loop_create_default());

        // 将默认工作站初始化为网络接口实例（esp-netif）
        esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
        assert(sta_netif);
        netif_initialized = true;
    }

    switch (scan_mode) {
    case SCAN:
        scan(scan_list_size);
        break;
    case FAST_SCAN:
        fast_scan(myssid, mypassword);
        break;
    default:
        break;
    }

    return ESP_OK;
}