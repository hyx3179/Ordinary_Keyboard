#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "esp_hidd_prf_api.h"
#include "hid_dev.h"
#include "Ordinary_Keyboard_main.h"

#define KM_TAG  "KM"
#define KEY_MAP(KEY) char2key[KEY-32]

const uint16_t char2key[] = { /**< 字符与键值对应表 */
    //键值                                                   二         十   十六   图形
    HID_KEY_SPACEBAR,                                //    00100000     32    20    " "
    HID_KEY_1 + (LEFT_SHIFT_KEY_MASK << 8),          //    00100001     33    21    "!"
    HID_KEY_SGL_QUOTE + (LEFT_SHIFT_KEY_MASK << 8),  //    00100010     34    22    """
    HID_KEY_3 + (LEFT_SHIFT_KEY_MASK << 8),          //    00100011     35    23    "#"
    HID_KEY_4 + (LEFT_SHIFT_KEY_MASK << 8),          //    00100100     36    24    "$"
    HID_KEY_5 + (LEFT_SHIFT_KEY_MASK << 8),          //    00100101     37    25    "%"
    HID_KEY_7 + (LEFT_SHIFT_KEY_MASK << 8),          //    00100110     38    26    "&"
    HID_KEY_SGL_QUOTE,                               //    00100111     39    27    "'"
    HID_KEY_9 + (LEFT_SHIFT_KEY_MASK << 8),          //    00101000     40    28    "("
    HID_KEY_0 + (LEFT_SHIFT_KEY_MASK << 8),          //    00101001     41    29    ")"
    HID_KEY_8 + (LEFT_SHIFT_KEY_MASK << 8),          //    00101010     42    2A    "*"
    HID_KEY_EQUAL + (LEFT_SHIFT_KEY_MASK << 8),      //    00101011     43    2B    "+"
    HID_KEY_COMMA,                                   //    00101100     44    2C    ","
    HID_KEY_MINUS,                                   //    00101101     45    2D    "-"
    HID_KEY_DOT,                                     //    00101110     46    2E    "."
    HID_KEY_FWD_SLASH,                               //    00101111     47    2F    "/"
    HID_KEY_0,                                       //    00110000     48    30    "0"
    HID_KEY_1,                                       //    00110001     49    31    "1"
    HID_KEY_2,                                       //    00110010     50    32    "2"
    HID_KEY_3,                                       //    00110011     51    33    "3"
    HID_KEY_4,                                       //    00110100     52    34    "4"
    HID_KEY_5,                                       //    00110101     53    35    "5"
    HID_KEY_6,                                       //    00110110     54    36    "6"
    HID_KEY_7,                                       //    00110111     55    37    "7"
    HID_KEY_8,                                       //    00111000     56    38    "8"
    HID_KEY_9,                                       //    00111001     57    39    "9"
    HID_KEY_SEMI_COLON + (LEFT_SHIFT_KEY_MASK << 8), //    00111010     58    3A    ":"
    HID_KEY_SEMI_COLON,                              //    00111011     59    3B    ";"
    HID_KEY_COMMA + (LEFT_SHIFT_KEY_MASK << 8),      //    00111100     60    3C    "<"
    HID_KEY_EQUAL,                                   //    00111101     61    3D    "="
    HID_KEY_DOT + (LEFT_SHIFT_KEY_MASK << 8),        //    00111110     62    3E    ">"
    HID_KEY_FWD_SLASH + (LEFT_SHIFT_KEY_MASK << 8),  //    00111111     63    3F    "?"
    HID_KEY_2 + (LEFT_SHIFT_KEY_MASK << 8),          //    01000000     64    40    "@"
    HID_KEY_A + (LEFT_SHIFT_KEY_MASK << 8),          //    01000001     65    41    "A"
    HID_KEY_B + (LEFT_SHIFT_KEY_MASK << 8),          //    01000010     66    42    "B"
    HID_KEY_C + (LEFT_SHIFT_KEY_MASK << 8),          //    01000011     67    43    "C"
    HID_KEY_D + (LEFT_SHIFT_KEY_MASK << 8),          //    01000100     68    44    "D"
    HID_KEY_E + (LEFT_SHIFT_KEY_MASK << 8),          //    01000101     69    45    "E"
    HID_KEY_F + (LEFT_SHIFT_KEY_MASK << 8),          //    01000110     70    46    "F"
    HID_KEY_G + (LEFT_SHIFT_KEY_MASK << 8),          //    01000111     71    47    "G"
    HID_KEY_H + (LEFT_SHIFT_KEY_MASK << 8),          //    01001000     72    48    "H"
    HID_KEY_I + (LEFT_SHIFT_KEY_MASK << 8),          //    01001001     73    49    "I"
    HID_KEY_J + (LEFT_SHIFT_KEY_MASK << 8),          //    01001010     74    4A    "J"
    HID_KEY_K + (LEFT_SHIFT_KEY_MASK << 8),          //    01001011     75    4B    "K"
    HID_KEY_L + (LEFT_SHIFT_KEY_MASK << 8),          //    01001100     76    4C    "L"
    HID_KEY_M + (LEFT_SHIFT_KEY_MASK << 8),          //    01001101     77    4D    "M"
    HID_KEY_N + (LEFT_SHIFT_KEY_MASK << 8),          //    01001110     78    4E    "N"
    HID_KEY_O + (LEFT_SHIFT_KEY_MASK << 8),          //    01001111     79    4F    "O"
    HID_KEY_P + (LEFT_SHIFT_KEY_MASK << 8),          //    01010000     80    50    "P"
    HID_KEY_Q + (LEFT_SHIFT_KEY_MASK << 8),          //    01010001     81    51    "Q"
    HID_KEY_R + (LEFT_SHIFT_KEY_MASK << 8),          //    01010010     82    52    "R"
    HID_KEY_S + (LEFT_SHIFT_KEY_MASK << 8),          //    01010011     83    53    "S"
    HID_KEY_T + (LEFT_SHIFT_KEY_MASK << 8),          //    01010100     84    54    "T"
    HID_KEY_U + (LEFT_SHIFT_KEY_MASK << 8),          //    01010101     85    55    "U"
    HID_KEY_V + (LEFT_SHIFT_KEY_MASK << 8),          //    01010110     86    56    "V"
    HID_KEY_W + (LEFT_SHIFT_KEY_MASK << 8),          //    01010111     87    57    "W"
    HID_KEY_X + (LEFT_SHIFT_KEY_MASK << 8),          //    01011000     88    58    "X"
    HID_KEY_Y + (LEFT_SHIFT_KEY_MASK << 8),          //    01011001     89    59    "Y"
    HID_KEY_Z + (LEFT_SHIFT_KEY_MASK << 8),          //    01011010     90    5A    "Z"
    HID_KEY_LEFT_BRKT,                               //    01011011     91    5B    "["
    HID_KEY_BACK_SLASH,                              //    01011100     92    5C    "\"
    HID_KEY_RIGHT_BRKT,                              //    01011101     93    5D    "]"
    HID_KEY_6 + (LEFT_SHIFT_KEY_MASK << 8),          //    01011110     94    5E    "^"
    HID_KEY_MINUS + (LEFT_SHIFT_KEY_MASK << 8),      //    01011111     95    5F    "_"
    HID_KEY_GRV_ACCENT,                              //    01100000     96    60    "`"
    HID_KEY_A,                                       //    01100001     97    61    "a"
    HID_KEY_B,                                       //    01100010     98    62    "b"
    HID_KEY_C,                                       //    01100011     99    63    "c"
    HID_KEY_D,                                       //    01100100    100    64    "d"
    HID_KEY_E,                                       //    01100101    101    65    "e"
    HID_KEY_F,                                       //    01100110    102    66    "f"
    HID_KEY_G,                                       //    01100111    103    67    "g"
    HID_KEY_H,                                       //    01101000    104    68    "h"
    HID_KEY_I,                                       //    01101001    105    69    "i"
    HID_KEY_J,                                       //    01101010    106    6A    "j"
    HID_KEY_K,                                       //    01101011    107    6B    "k"
    HID_KEY_L,                                       //    01101100    108    6C    "l"
    HID_KEY_M,                                       //    01101101    109    6D    "m"
    HID_KEY_N,                                       //    01101110    110    6E    "n"
    HID_KEY_O,                                       //    01101111    111    6F    "o"
    HID_KEY_P,                                       //    01110000    112    70    "p"
    HID_KEY_Q,                                       //    01110001    113    71    "q"
    HID_KEY_R,                                       //    01110010    114    72    "r"
    HID_KEY_S,                                       //    01110011    115    73    "s"
    HID_KEY_T,                                       //    01110100    116    74    "t"
    HID_KEY_U,                                       //    01110101    117    75    "u"
    HID_KEY_V,                                       //    01110110    118    76    "v"
    HID_KEY_W,                                       //    01110111    119    77    "w"
    HID_KEY_X,                                       //    01111000    120    78    "x"
    HID_KEY_Y,                                       //    01111001    121    79    "y"
    HID_KEY_Z,                                       //    01111010    122    7A    "z"
    HID_KEY_LEFT_BRKT + (LEFT_SHIFT_KEY_MASK << 8),  //    01111011    123    7B    "{"
    HID_KEY_BACK_SLASH + (LEFT_SHIFT_KEY_MASK << 8), //    01111100    124    7C    "|"
    HID_KEY_RIGHT_BRKT + (LEFT_SHIFT_KEY_MASK << 8), //    01111101    125    7D    "}"
    HID_KEY_GRV_ACCENT + (LEFT_SHIFT_KEY_MASK << 8)  //    01111110    126    7E    "~"
};

void post_item(uint8_t *key_vaule)
{
    extern xQueueHandle keyboard_queue;
    ESP_LOGD(KM_TAG, "the key vaule = %d, %d, %d, %d, %d, %d, %d, %d", key_vaule[0],
             key_vaule[1], key_vaule[2], key_vaule[3], key_vaule[4], key_vaule[5], key_vaule[6], key_vaule[7]);
    xQueueSendToBack(keyboard_queue, key_vaule, 0);
    memset(key_vaule, 0, HID_KEYBOARD_IN_RPT_LEN);
    xQueueSendToBack(keyboard_queue, key_vaule, 0);
}

int STR_cmd_handle(uint8_t *str)
{
    //ESP_LOGD(KM_TAG, "STR_cmd_handle(%s)", str);
    uint8_t key_vaule[HID_KEYBOARD_IN_RPT_LEN];
    memset(&key_vaule, 0, HID_KEYBOARD_IN_RPT_LEN);
    bool lowercase = false, capital = false;

    for (int i = 0, n = 2;; i++) {
        if (n == HID_KEYBOARD_IN_RPT_LEN) {
            post_item(key_vaule);
            n = 2;
        }
        switch (str[i]) {
        case 0:
            post_item(key_vaule);
            return i;
        case '\\':
            post_item(key_vaule);
            return i - 1;
        default:
            if (KEY_MAP(str[i]) > 255) {
                if (n > 2 && lowercase) {
                    post_item(key_vaule);
                    lowercase = false;
                    n = 2;
                }
                key_vaule[0] += KEY_MAP(str[i]) >> 8;
                key_vaule[n] = KEY_MAP(str[i]);
                n++;
                capital = true;
                if (key_vaule[n - 2] == key_vaule[n - 1]) {
                    key_vaule[n - 1] = 0;
                    post_item(key_vaule);
                    n = 2;
                    i--;
                }
            } else {
                if (n > 2 && capital) {
                    post_item(key_vaule);
                    capital = false;
                    n = 2;
                }
                key_vaule[n] = KEY_MAP(str[i]);
                n++;
                lowercase = true;
                if (key_vaule[n - 2] == key_vaule[n - 1]) {
                    key_vaule[n - 1] = 0;
                    post_item(key_vaule);
                    n = 2;
                    i--;
                }
            }
            break;
        }
    }
}

esp_err_t keyboard_macro_handle(uint8_t *macro, int len)
{
    uint8_t key_vaule[HID_KEYBOARD_IN_RPT_LEN];
    memset(&key_vaule, 0, HID_KEYBOARD_IN_RPT_LEN);

    for (int i = 0, n = 2; i < len; i++) {
        if (n == HID_KEYBOARD_IN_RPT_LEN) {
            return ESP_ERR_INVALID_ARG;
        }
        switch ((macro[i] << 16) + (macro[i + 1] << 8) + macro[i + 2]) {
        case STR:
            i += 3;
            i += STR_cmd_handle(&macro[i]);
            break;
        case ESCAPE:
            key_vaule[n] = HID_KEY_ESCAPE;
            n++;
            i += 2;
            break;
        case TAB:
            key_vaule[n] = HID_KEY_TAB;
            n++;
            i += 2;
            break;
        case CAPSLOCK:
            key_vaule[n] = HID_KEY_CAPS_LOCK;
            n++;
            i += 2;
            break;
        case ENTER:
            key_vaule[n] = HID_KEY_ENTER;
            n++;
            i += 2;
            break;
        case BACKSPACE:
            key_vaule[n] = HID_KEY_DELETE;
            n++;
            i += 2;
            break;
        case LEFTCONTORL:
            key_vaule[0] += LEFT_CONTROL_KEY_MASK;
            i += 2;
            break;
        case LEFTSHIFT:
            key_vaule[0] += LEFT_SHIFT_KEY_MASK;
            i += 2;
            break;
        case LEFTALT:
            key_vaule[0] += LEFT_ALT_KEY_MASK;
            i += 2;
            break;
        case LEFTGUI:
            key_vaule[0] += LEFT_GUI_KEY_MASK;
            i += 2;
            break;
        case RIGHTCONTORL:
            key_vaule[0] += RIGHT_CONTROL_KEY_MASK;
            i += 2;
            break;
        case RIGHTSHIFT:
            key_vaule[0] += RIGHT_SHIFT_KEY_MASK;
            i += 2;
            break;
        case RIGHTALT:
            key_vaule[0] += RIGHT_ALT_KEY_MASK;
            i += 2;
            break;
        case RIGHTGUI:
            key_vaule[0] += RIGHT_GUI_KEY_MASK;
            i += 2;
            break;
        default:
            if (i == 0) {
                return ESP_ERR_INVALID_ARG;
            }
            if (macro[i] == '\\') {
                key_vaule[2] = KEY_MAP('\\');
                post_item(key_vaule);
                i++;
                i += STR_cmd_handle(&macro[i]);
                break;
            }
            key_vaule[n] = KEY_MAP(macro[i]);
            n++;
        }
        if (macro[i + 1] == '/') {
            post_item(key_vaule);
            i++;
            n = 2;
        }
    }
    return ESP_OK;
}