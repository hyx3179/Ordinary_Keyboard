#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_system.h"
#include "freertos/queue.h"

#include "esp_hidd_prf_api.h"


void keyboard_macro(char* macro, int len);