import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_NAME, UNIT_MILLIMETER, ICON_RULER, CONF_UPDATE_INTERVAL

aj_sr04m_ns = cg.esphome_ns.namespace("aj_sr04m")
AJ_SR04MSensor = aj_sr04m_ns.class_("AJ_SR04M_Sensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_MILLIMETER,
    icon=ICON_RULER,
    accuracy_decimals=2,
).extend({
    cv.GenerateID(): cv.declare_id(AJ_SR04MSensor),
    cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
}).extend(cv.polling_component_schema("30s"))


async def to_code(config):
    uart = await cg.get_variable(config["uart_id"])
    var = cg.new_Pvariable(config[CONF_ID], uart)
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)



# Example yaml

# external_components:
#   - source:
#       type: git
#       url: https://github.com/meehatpa/esphome-external-components
#       ref: main
#
#
# uart:
#   id: uart_bus
#   tx_pin: GPIO1
#   rx_pin: GPIO3
#   baud_rate: 9600
#
# sensor:
#   - platform: aj_sr04m
#     name: "Distance"
#     uart_id: uart_bus
#     update_interval: 30min
