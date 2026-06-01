import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID, CONF_NAME

from . import Daikin200Climate

DEPENDENCIES = ["climate"]

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Daikin200Climate),
}).extend(climate.CLIMATE_SCHEMA).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)
