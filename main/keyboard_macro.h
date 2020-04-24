
#define KM_TAG  "KM"
#define KEY_MAP(KEY) char2key[KEY-32]
#define KEY_VAULE_LEN 8

//宏命令

#define STR          6050644  //'\\ST'   // String
#define ESCAPE       '\\ES'   // Keyboard ESCAPE
#define TAB          '\\TA'   // Keyboard Tab
#define CAPSLOCK     '\\CL'   // Keyboard Caps Lock
#define LEFTCONTORL  '\\LC'   // Keyboard LeftContorl
#define LEFTSHIFT    '\\LS'   // Keyboard LeftShift
#define LEFTALT      '\\LA'   // Keyboard LeftAlt
#define LEFTGUI      '\\LG'   // Keyboard LeftGUI
#define RIGHTCONTORL '\\RC'   // Keyboard RightContorl
#define RIGHTSHIFT   '\\RS'   // Keyboard RightShift
#define RIGHTALT     '\\RA'   // Keyboard RightAlt
#define RIGHTGUI     '\\RG'   // Keyboard RightGUI
#define ENTER        6047054  //'\\EN'   // Keyboard Return (ENTER)
#define BACKSPACE    '\\BA'   // Keyboard DELETE (Backspace)

//void keyboard_macro(uint8_t *macro, int len);

/**
 * @brief  发送键值
 *         将键值发布到队列的后面，等待发送
 * @param  key_vaule: 键值数组的指针
 * //@return              ESP_OK - success, other - failed
 */
void post_item(uint8_t *key_vaule);

/**
 * @brief  字符串命令处理
 * @param  str: 字符串的指针
 * @return 处理字符数量
 */
int STR_cmd_handle(uint8_t *str);

/**
 * @brief  键盘宏处理程序
 * @param  macro: 宏字符串的指针
 * @param  len: 宏字符串的长度
 * @return      ESP_OK - success, other - failed
 */
esp_err_t keyboard_macro_handle(uint8_t *macro, int len);
