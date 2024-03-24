# ESP32-C6 + BME280 Zigbee Climate Sensor
This code is intended for an ESP32-C6 + BME280 running on battery, automatically connects and reports to a zigbee coordinator, while remaining in deep sleep between reports. To further preserve battery, the zigbee stack is only started if the temperature changes more than 0.5c, or if attributes haven't reported in the last 10 deep sleep cycles.

* Using a Sonoff Zigbee 3.0 USB Dongle Plus and Zigbee2MQTT to connect to HomeAssistant


### BME280
* For the sensor reading, implements a minimally modified version of 'BMX280 for ESP-IDF' (Halit Utku Maden)
* https://github.com/utkumaden/esp-idf-bmx280/tree/master


### Zigbee
* Basic zigbee connectivity based on esp-idf zigbee examples
* https://github.com/espressif/esp-idf/tree/master/examples/zigbee/light_sample/HA_on_off_light
* Modified and added functions for custom manufacturer/model, climate clusters, and attribute reporting


### Deep Sleep
* Wakeup from deep sleep by RTC clock
* To prevent running continually and killing the battery (in the case of a sensor/zigbee/other task fault), a task runs in the background, triggering deep sleep after a set time


### Zigbee2MQTT / Home Assistant
* Zigbee2MQTT .js herdsman converter definition
    * Set model to match zigbee basic cluster (in zb_main.c)
    * Report configuration (ie attribute reporting intervals) are defined here
* Home Assistant configuration.yaml - exposes zigbee/MQTT attributes as entities in Home Assistant
  ```
  mqtt:
    - sensor:
        name: "ESP32C6 Temperature"
        state_topic: "zigbee2mqtt/0x404ccafffe44d760"
        unit_of_measurement: "°C"
        value_template: "{{ value_json.temperature }}"
        icon: mdi:thermometer-lines
  ```
