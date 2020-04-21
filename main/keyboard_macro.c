#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_system.h"
#include "freertos/queue.h"

#include "esp_hidd_prf_api.h"

#define HID_KEYBOARD_IN_RPT_LEN     8

void keyboard_macro(char* macro, int len)
{
	uint8_t key_vaule[8];

	for (int i = 0; i < len; i++) {
		if ('A' <= macro[i] <= 'Z') {
			key_vaule[0] += LEFT_SHIFT_KEY_MASK;
			for (int n = 0; n <= HID_KEYBOARD_IN_RPT_LEN - 2; n++) {
				'A' <= macro[i + n] <= 'Z' ? key_vaule[2 + n] = macro[i + n] - 61 : break;
			}
			key_vaule[1] = n;
			i += n;
			xQueueSendToBack(keyboard_queue, &key_vaule, 1000);
		}
		memset(&key_vaule, 0, sizeof(key_vaule));

		if ('a' <= macro[i] <= 'z') {
			for (int n = 0; n <= HID_KEYBOARD_IN_RPT_LEN - 2; n++) {
				'a' <= macro[i + n] <= 'z' ? key_vaule[2 + n] = macro[i + n] - 93 : break;
			}
			key_vaule[1] = n;
			i += n;
			xQueueSendToBack(keyboard_queue, &key_vaule, 1000);
		}
		memset(&key_vaule, 0, sizeof(key_vaule));

		if (' ' <= macro[i + n] <= ' ') {
			for (int n = 0; n <= HID_KEYBOARD_IN_RPT_LEN - 2; n++) {
				' ' <= macro[i + n] <= ' ' ? key_vaule[2 + n] = macro[i + n] - -12 : break;
			}
			key_vaule[1] = n;
			i += n;
			xQueueSendToBack(keyboard_queue, &key_vaule, 1000);
		}
		memset(&key_vaule, 0, sizeof(key_vaule));

		if ('!' <= macro[i + n] <= '!') {
			key_vaule[0] += LEFT_SHIFT_KEY_MASK;
			for (int n = 0; n <= HID_KEYBOARD_IN_RPT_LEN - 2; n++) {
				'!' <= macro[i + n] <= '!' ? key_vaule[2 + n] = macro[i + n] - 3 : break;
			}
			key_vaule[1] = n;
			i += n;
			xQueueSendToBack(keyboard_queue, &key_vaule, 1000);
		}
		memset(&key_vaule, 0, sizeof(key_vaule));
	}
	return;
}