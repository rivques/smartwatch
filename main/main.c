#include <stdio.h>
#include "display_handler.h"
#include "time_mgmt.h"
#include "freertos/FreeRTOS.h"
#include <time.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <string.h>
#include "sdkconfig.h"
#include "esp_lvgl_port.h"

static const char *TAG = "MAIN";

const char *day_of_week[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *month_of_year[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void app_main(void)
{
    ESP_LOGI(TAG, "Starting application");
    wifi_init();
    ESP_LOGI(TAG, "WiFi initialized");
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    ESP_LOGI(TAG, "Connected to WiFi...");

    init_all_display();
    ESP_LOGI(TAG, "Display initialized");
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        ntp_sync();
        // update 'now' variable with current time
        time(&now);
    }
    ESP_LOGI(TAG, "Current time: %s", ctime(&now));
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    char strftime_buf[64];
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in New York is: %s", strftime_buf);

    while (1)
    {
        vTaskDelay(200 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        ESP_LOGI(TAG, "The current date/time in New York is: %s", strftime_buf);
        lvgl_port_lock(0);
        update_time_display(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        char day_str[4];
        snprintf(day_str, sizeof(day_str), "%02d", timeinfo.tm_mday);
        char year_str[12];
        snprintf(year_str, sizeof(year_str), "%04d", timeinfo.tm_year + 1900);
        update_date_display(month_of_year[timeinfo.tm_mon], day_str, year_str, day_of_week[timeinfo.tm_wday]);
        strftime(strftime_buf, sizeof(strftime_buf), "%I:%M:%S%p", &timeinfo);
        update_status_bar(strftime_buf, 70);
        lvgl_port_unlock();
    }
    
}