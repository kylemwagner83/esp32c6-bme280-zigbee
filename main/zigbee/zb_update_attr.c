#include "ha/esp_zigbee_ha_standard.h"

esp_err_t zb_update_temperature(int32_t temperature)
{
    /* Update temperature attribute */
    esp_zb_zcl_status_t state = esp_zb_zcl_set_attribute_val(
        HA_ESP_SENSOR_ENDPOINT,
        ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
        ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID,
        &temperature,
        false
    );

    /* Error check */
    if(state != ESP_ZB_ZCL_STATUS_SUCCESS)
    {
        ESP_LOGE(TAG, "Updating temperature attribute failed!");
        return ESP_FAIL;
    }
    
    return ESP_OK;
}


esp_err_t zb_update_humidity(int32_t humidity)
{
    /* Update humidity attribute */
    esp_zb_zcl_status_t state = esp_zb_zcl_set_attribute_val(
        HA_ESP_SENSOR_ENDPOINT,
        ESP_ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT,
        ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        ESP_ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID,
        &humidity,
        false
    );

    /* Error check */
    if(state != ESP_ZB_ZCL_STATUS_SUCCESS)
    {
        ESP_LOGE(TAG, "Updating humidity attribute failed!");
        return ESP_FAIL;
    }

    return ESP_OK;
}


esp_err_t zb_update_pressure(int32_t pressure)
{
    /* Update pressure attribute */
    esp_zb_zcl_status_t state = esp_zb_zcl_set_attribute_val(
        HA_ESP_SENSOR_ENDPOINT,
        ESP_ZB_ZCL_CLUSTER_ID_PRESSURE_MEASUREMENT,
        ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        ESP_ZB_ZCL_ATTR_PRESSURE_MEASUREMENT_VALUE_ID,
        &pressure,
        false
    );

    /* Error check */
    if(state != ESP_ZB_ZCL_STATUS_SUCCESS)
    {
        ESP_LOGE(TAG, "Updating pressure attribute failed!");
        return ESP_FAIL;
    }
    
    return ESP_OK;
}