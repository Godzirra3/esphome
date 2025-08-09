import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_MOTION,
    DEVICE_CLASS_MOTION,
)
from . import CONF_DFROBOT_SEN0623_ID, DfrobotSen0623Component

DEPENDENCIES = ["dfrobot_sen0623"]

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(CONF_DFROBOT_SEN0623_ID): cv.use_id(DfrobotSen0623Component),
            cv.Optional(CONF_MOTION): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_MOTION
            ),
        }
    )
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_DFROBOT_SEN0623_ID])

    if motion := config.get(CONF_MOTION):
        sens = await binary_sensor.new_binary_sensor(motion)
        cg.add(parent.set_motion_binary_sensor(sens))