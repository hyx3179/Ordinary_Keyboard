idf_component_register(SRCS "Ordinary_Keyboard_main.c"
                            "configure_server.c"
                            "keyboard_macro.c"
                            "wifi_connect.c"
                            "start_ble_hid_server.c"
                            "esp_hidd_prf_api.c"
                            "hid_dev.c"
                            "hid_device_le_prf.c"
                       INCLUDE_DIRS "."
                       EMBED_FILES "favicon.ico" "upload_script.html")

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-const-variable)