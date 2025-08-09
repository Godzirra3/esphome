import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    DEVICE_CLASS_EMPTY,
    STATE_CLASS_MEASUREMENT,
    UNIT_BEATS_PER_MINUTE,
    CONF_RATE
)
from . import CONF_DFROBOT_SEN0623_ID, DfrobotSen0623Component

DEPENDENCIES = ["dfrobot_sen0623"]

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(CONF_DFROBOT_SEN0623_ID): cv.use_id(DfrobotSen0623Component),
            cv.Optional(CONF_RATE): sensor.sensor_schema(
                unit_of_measurement=UNIT_BEATS_PER_MINUTE,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_EMPTY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_DFROBOT_SEN0623_ID])

    if rate := config.get(CONF_RATE):
        sens = await sensor.new_sensor(rate)
        cg.add(parent.set_heart_rate_sensor(sens))