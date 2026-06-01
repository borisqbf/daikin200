import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

from . import Daikin200Climate

DEPENDENCIES = ["climate"]

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Daikin200Climate),
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[cv.ID])
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)
    