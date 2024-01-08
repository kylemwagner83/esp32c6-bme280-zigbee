#include "ha/esp_zigbee_ha_standard.h"

static esp_zb_zcl_report_attr_cmd_t temperature_cmd_req = {
    .zcl_basic_cmd.src_endpoint = 1,
    .address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT,
    .clusterID = ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
    .cluster_role = ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
    .attributeID = ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID
};

esp_err_t zb_update_temperature(int32_t temperature)
{
    esp_zb_zcl_status_t state = esp_zb_zcl_set_attribute_val(
        HA_ESP_SENSOR_ENDPOINT,
        ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
        ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID,
        &temperature,
        false
    );

     /* Check for error */
    if(state != ESP_ZB_ZCL_STATUS_SUCCESS)
    {
        ESP_LOGE(TAG, "Setting temperature attribute failed!");
        return ESP_FAIL;
    }

    /* Request sending new temperature */
    state = esp_zb_zcl_report_attr_cmd_req(&temperature_cmd_req);

    /* Check for error */
    if(state != ESP_ZB_ZCL_STATUS_SUCCESS)
    {
        ESP_LOGE(TAG, "Sending temperature attribute report command failed!");
        return ESP_FAIL;
    }
    
    return ESP_OK;
}