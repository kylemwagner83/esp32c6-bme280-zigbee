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


RTC_NOINIT_ATTR uint8_t loopCount;
RTC_NOINIT_ATTR struct bmeValsStruct bmeValsRTCmem;
struct bmeValsStruct bmeVals;


static void esp_deep_sleep_countdown(void *params)
{
    vTaskDelay((30 * 1000) / portTICK_PERIOD_MS); // Delay time in milliseconds (seconds * 1000)
    printf("App timed out - Entering deep sleep\n");
    esp_deep_sleep_start();
}

static void update_attributes()
{
    /* Start Zigbee */
    ESP_ERROR_CHECK(zb_start());

    /* Wait for Zigbee connection */
    vTaskDelay((10 * 1000) / portTICK_PERIOD_MS); // Delay time in milliseconds (seconds * 1000)
    ESP_LOGI(TAG, "10 seconds after zb_start");

    bmeValsRTCmem.convertedTemp = bmeVals.convertedTemp;
    ESP_ERROR_CHECK(zb_update_temperature(bmeVals.convertedTemp));

    vTaskDelay((1 * 1000) / portTICK_PERIOD_MS); // Delay time in milliseconds (seconds * 1000)
    printf("Attributes updated - Entering deep sleep\n");
    esp_deep_sleep_start();
}


void app_main(void)
{
    /* Enable wakeup from deep sleep by rtc timer */
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(5 * 1000000)); // Sleep time in microseconds (seconds * 1000000)

    /* Start failsafe timer - always sleep/restart after time expires */
    xTaskCreate(esp_deep_sleep_countdown, "Deep sleep countdown", 4096, NULL, 2, NULL);

    /* Get values from BME280 */
    get_bme_vals();

    /* Check number of restarts since last update, update if >=5 */
    ESP_LOGI("app_main", "Loopcount = %i", loopCount);
    if (loopCount >= 5)
    {
        printf("Reached max loopcount - Updating attributes\n");
        loopCount = 0; 
        update_attributes();
    }
    else{loopCount = loopCount + 1;}
    
    /* Check temperature change since last update, update if +/- 1.5c */
    ESP_LOGI("app_main", "Last temp = %i, Current temp = %i", bmeValsRTCmem.convertedTemp, bmeVals.convertedTemp);
    if (bmeVals.convertedTemp <= (bmeValsRTCmem.convertedTemp - 150) || bmeVals.convertedTemp >= (bmeValsRTCmem.convertedTemp + 150))
    {
        printf("Temp changed - Updating attributes\n");
        loopCount = 0;
        update_attributes();
    }
    

    printf("Nothing to do - Entering deep sleep\n");
    esp_deep_sleep_start();
}