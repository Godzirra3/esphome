import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import DEVICE_CLASS_MOTION

from . import CONF_ID, DfrobotSen0623Component  # Use correct imports

DEPENDENCIES = ["dfrobot_sen0623"]

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(
    device_class=DEVICE_CLASS_MOTION
    ).extend(
    {
        cv.GenerateID(): cv.use_id(DfrobotSen0623Component),
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_ID])
    binary_sens = await binary_sensor.new_binary_sensor(config)

    cg.add(parent.set_detected_binary_sensor(binary_sens))
