const fz = require('zigbee-herdsman-converters/converters/fromZigbee');
const tz = require('zigbee-herdsman-converters/converters/toZigbee');
const exposes = require('zigbee-herdsman-converters/lib/exposes');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
const extend = require('zigbee-herdsman-converters/lib/extend');
const e = exposes.presets;
const ea = exposes.access;

const definition = {
    zigbeeModel: ['ESP32C6.Sensor'],
    model: 'ESP32C6', 
    vendor: 'Espressif', 
    description: 'ESP32C6 Sensor',
    fromZigbee: [fz.humidity, fz.temperature, fz.pressure],
    toZigbee: [], // Should be empty, unless device can be controlled (e.g. lights, switches).
    configure: async (device, coordinatorEndpoint, logger) => {
        const endpoint = device.getEndpoint(1);
        await reporting.bind(endpoint, coordinatorEndpoint, ['msTemperatureMeasurement']);
        await reporting.bind(endpoint, coordinatorEndpoint, ['msRelativeHumidity']);
        await reporting.bind(endpoint, coordinatorEndpoint, ['msPressureMeasurement']);
        await reporting.temperature(endpoint);
        await reporting.humidity(endpoint);
        await reporting.pressure(endpoint);
    },
    exposes: [e.temperature(), e.humidity(), e.pressure()],     
};

module.exports = definition;