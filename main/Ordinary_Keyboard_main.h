

// ������
// ASCII�벻����ʾ�ַ�
#define STR          6050644  //'\\ST'   // String
#define ESCAPE       6047059  //'\\ES'   // Keyboard ESCAPE
#define TAB          6050881  //'\\TA'   // Keyboard Tab
#define CAPSLOCK     6046540  //'\\CL'   // Keyboard Caps Lock
#define ENTER        6047054  //'\\EN'   // Keyboard Return (ENTER)
#define BACKSPACE    6046273  //'\\BA'   // Keyboard DELETE (Backspace)
// ���μ�
#define LEFTCONTORL  6048835  //'\\LC'   // Keyboard LeftContorl
#define LEFTSHIFT    6048851  //'\\LS'   // Keyboard LeftShift
#define LEFTALT      6048833  //'\\LA'   // Keyboard LeftAlt
#define LEFTGUI      6048839  //'\\LG'   // Keyboard LeftGUI
#define RIGHTCONTORL 6050371  //'\\RC'   // Keyboard RightContorl
#define RIGHTSHIFT   6050387  //'\\RS'   // Keyboard RightShift
#define RIGHTALT     6050369  //'\\RA'   // Keyboard RightAlt
#define RIGHTGUI     6050375  //'\\RG'   // Keyboard RightGUI

// wifi ɨ��ģʽ
enum SCAN_MODE {
    SCAN,
    FAST_SCAN,
};

/**
 * @brief  ���ͼ�ֵ
 *         ����ֵ���������еĺ��棬�ȴ�����
 * @param  key_vaule: ��ֵ�����ָ��
 */
void post_item(uint8_t *key_vaule);

/**
 * @brief  �ַ��������
 * @param  str: �ַ�����ָ��
 * @return �����ַ�����
 */
int STR_cmd_handle(uint8_t *str);

/**
 * @brief  ���̺괦�����
 * @param  macro: ���ַ�����ָ��
 * @param  len: ���ַ����ĳ���
 * @return      ESP_OK - success, other - failed
 */
esp_err_t keyboard_macro_handle(uint8_t *macro, int len);

/**
* ���ԡ������ͷ��/����β
* Ҫ���롰/�����á���������
* \\ST ����
*/

esp_err_t start_ble_hid_server();

esp_err_t configure_server(const char *base_path);

esp_err_t wifi_connect(int scan_mode, uint8_t *myssid, uint8_t *mypassword);
