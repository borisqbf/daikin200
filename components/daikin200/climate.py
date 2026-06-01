import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

from esphome.const import CONF_ID, CONF_NAME

from . import Daikin200Climate

CONFIG_SCHEMA = climate._CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Daikin200Climate),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    tx = yield cg.get_variable(config["remote_transmitter"])
    cg.add(var.set_transmitter(tx))

    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)