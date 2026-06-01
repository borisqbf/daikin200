import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, remote_transmitter
from esphome.const import CONF_ID

from . import Daikin200Climate

DEPENDENCIES = ["climate"]

CONFIG_SCHEMA = climate._CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Daikin200Climate),
    cv.Required("remote_transmitter"): cv.use_id(cg.Component),
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    tx = await cg.get_variable(config["remote_transmitter"])
    cg.add(var.set_transmitter(tx))

    await cg.register_component(var, config)
    await climate.register_climate(var, config)