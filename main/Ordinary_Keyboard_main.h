

// 宏命令
// ASCII码不可显示字符
#define STR          6050644  //'\\ST'   // String
#define ESCAPE       6047059  //'\\ES'   // Keyboard ESCAPE
#define TAB          6050881  //'\\TA'   // Keyboard Tab
#define CAPSLOCK     6046540  //'\\CL'   // Keyboard Caps Lock
#define ENTER        6047054  //'\\EN'   // Keyboard Return (ENTER)
#define BACKSPACE    6046273  //'\\BA'   // Keyboard DELETE (Backspace)
// 修饰键
#define LEFTCONTORL  6048835  //'\\LC'   // Keyboard LeftContorl
#define LEFTSHIFT    6048851  //'\\LS'   // Keyboard LeftShift
#define LEFTALT      6048833  //'\\LA'   // Keyboard LeftAlt
#define LEFTGUI      6048839  //'\\LG'   // Keyboard LeftGUI
#define RIGHTCONTORL 6050371  //'\\RC'   // Keyboard RightContorl
#define RIGHTSHIFT   6050387  //'\\RS'   // Keyboard RightShift
#define RIGHTALT     6050369  //'\\RA'   // Keyboard RightAlt
#define RIGHTGUI     6050375  //'\\RG'   // Keyboard RightGUI

// wifi 扫描模式
enum SCAN_MODE {
    SCAN,
    FAST_SCAN,
};

/**
 * @brief  发送键值
 *         将键值发布到队列的后面，等待发送
 * @param  key_vaule: 键值数组的指针
 */
void post_item(uint8_t *key_vaule);

/**
 * @brief  键盘宏处理程序
 * @param  macro: 宏字符串的指针
 * @param  len: 宏字符串的长度
 * @param  fb: 宏文件指针
 * @return      ESP_OK - success, other - failed
 */
esp_err_t keyboard_macro_handle(uint8_t *macro, int len, FILE *fd);

/**
* 宏以“命令”开头“/”结尾
* 要输入“/”需用“？”代替
* \\ST 命令
*/

esp_err_t start_ble_hid_server();

esp_err_t configure_server(const char *base_path);

esp_err_t wifi_connect(int scan_mode, uint8_t *myssid, uint8_t *mypassword);
