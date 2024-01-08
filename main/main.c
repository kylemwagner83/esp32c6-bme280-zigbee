/* Zigbee Libraries */
#include "ha/esp_zigbee_ha_standard.h"
#include "nvs_flash.h"

/* FreeRTOS Libraries */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Deep Sleep Library */
#include "esp_sleep.h"

/* Zigbee Code */
#include "zigbee/zb_main.c"
#include "zigbee/zb_update_attr.c"

/* BMX280 Code */
#include "bmx280/bmx280_main.c"
#include "bmx280/bmx280.c"


float temp = 0, pres = 0, hum = 0;
struct bmeValsStruct bmeVals;


void deep_sleep_timer_wakeup(void)
{
    const int wakeup_time_sec = 5;
    printf("Enabling timer wakeup, %ds\n", wakeup_time_sec);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000));
}

void app_main(void)
{
    /* Enable wakeup from deep sleep by rtc timer */
    deep_sleep_timer_wakeup();

    /* Start Zigbee */
    ESP_ERROR_CHECK(zb_start());

    /* Wait for Zigbee connection */
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "10 seconds after zb_start");

    /* Get values from BME sensor */
    get_bme_vals();

    /* Update temperature attribute */
    ESP_ERROR_CHECK(zb_update_temperature(bmeVals.convertedTemp));
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    printf("Entering deep sleep\n");
    esp_deep_sleep_start();
}