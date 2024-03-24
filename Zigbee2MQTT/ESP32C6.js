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
        await reporting.bind(endpoint, coordinatorEndpoint, ['msTemperatureMeasurement', 'msRelativeHumidity', 'msPressureMeasurement']);


        await reporting.temperature(endpoint);
        const temperature_payload = [{
            attribute: 'measuredValue',
            minimumReportInterval: 10,
            maximumReportInterval: 600,
            reportableChange: 100,
        }];
        await endpoint.configureReporting('msTemperatureMeasurement', temperature_payload);
        

        await reporting.humidity(endpoint);
        const humidity_payload = [{
            attribute: 'measuredValue',
            minimumReportInterval: 10,
            maximumReportInterval: 600,
            reportableChange: 100,
        }];
        await endpoint.configureReporting('msRelativeHumidity', humidity_payload);


        await reporting.pressure(endpoint);
        const pressure_payload = [{
            attribute: 'measuredValue',
            minimumReportInterval: 10,
            maximumReportInterval: 600,
            reportableChange: 5,
        }];
        await endpoint.configureReporting('msPressureMeasurement', pressure_payload);


    },
    exposes: [e.temperature(), e.humidity(), e.pressure()],     
};

module.exports = definition;