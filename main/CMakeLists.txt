idf_component_register(SRCS "time_mgmt.c" "pixeboy_20.c" "pixeboy_32.c" "bat.c" "lcd_7x5.c" "display_handler.c" "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_wifi esp_event nvs_flash)

# Enable Kconfig configuration
set(CONFIG_FILE sdkconfig)
