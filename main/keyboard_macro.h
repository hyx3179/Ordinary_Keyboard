
#define KM_TAG  "KM"
#define KEY_MAP(KEY) char2key[KEY-32]
#define KEY_VAULE_LEN 8

//������

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
 * @brief  ���ͼ�ֵ
 *         ����ֵ���������еĺ��棬�ȴ�����
 * @param  key_vaule: ��ֵ�����ָ��
 * //@return              ESP_OK - success, other - failed
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
