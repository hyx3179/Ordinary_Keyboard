

typedef struct KEY_PROPERTY {
    bool        status;               // 是否按下
    uint8_t     usage_id;             // Usage ID
    char        assic;                // assic
    uint8_t     byte_position;        // 字节位置
} key_property;

esp_err_t start_ble_hid_server();

esp_err_t configure_key_scan_array();

void keyboard_scan();