/* Zigbee Libraries */
#include "ha/esp_zigbee_ha_standard.h"
#include "nvs_flash.h"
#include "zcl/esp_zigbee_zcl_command.h"
#include "zcl/esp_zigbee_zcl_common.h"

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

/* Header */
#include "main.h"

/* Variables */
RTC_NOINIT_ATTR uint8_t loopCount;
RTC_NOINIT_ATTR struct bmeValsStruct bmeValsRTCmem;
struct bmeValsStruct bmeVals;


static void esp_deep_sleep_countdown(void *params)
{
    vTaskDelay((40 * 1000) / portTICK_PERIOD_MS); // Delay time in milliseconds (seconds * 1000)
    printf("App timed out - Entering deep sleep\n");
    esp_deep_sleep_start();
}


static void update_attributes()
{
    /* Start Zigbee, update temperature, humidity, and pressure attributes */
    ESP_ERROR_CHECK(zb_start());

    bmeValsRTCmem.convertedTemp = bmeVals.convertedTemp;
    ESP_ERROR_CHECK(zb_update_temperature(bmeVals.convertedTemp));

    bmeValsRTCmem.convertedHum = bmeVals.convertedHum;
    ESP_ERROR_CHECK(zb_update_humidity(bmeVals.convertedHum));

    bmeValsRTCmem.convertedPres = bmeVals.convertedPres;
    ESP_ERROR_CHECK(zb_update_pressure(bmeVals.convertedPres));

    printf("Waiting 20 seconds for zigbee connection and reporting\n");
    vTaskDelay((20 * 1000) / portTICK_PERIOD_MS); // Delay time in milliseconds (seconds * 1000)
}


void app_main(void)
{
    /* Enable wakeup from deep sleep by rtc timer - set deep sleep time */
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(60 * 1000000)); // Sleep time in microseconds (seconds * 1000000)

    /* Start failsafe timer - always sleep/restart after time expires */
    xTaskCreate(esp_deep_sleep_countdown, "Deep sleep countdown", 4096, NULL, 2, NULL);

    /* Get values from BME280 */
    get_bme_vals();

    /* Check number of restarts since last update, update if >=10 */
    ESP_LOGI("app_main", "Loopcount = %i", loopCount);
    if (loopCount >= 10)
    {
        printf("Reached max loopcount - Updating attributes\n");
        loopCount = 0;
        update_attributes();
    }
    else
    {
        loopCount = loopCount + 1;
        /* Check temperature change since last update, update if +/- 0.5c */
        ESP_LOGI("app_main", "Last temp = %i, Current temp = %i", bmeValsRTCmem.convertedTemp, bmeVals.convertedTemp);
        if (bmeVals.convertedTemp <= (bmeValsRTCmem.convertedTemp - 50) || bmeVals.convertedTemp >= (bmeValsRTCmem.convertedTemp + 50))
        {
            printf("Temp changed - Updating attributes\n");
            loopCount = 0;
            update_attributes();
        }
    }

    printf("Entering deep sleep\n");
    esp_deep_sleep_start();
}