import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

from . import daikin200_ns, Daikin200Climate

DEPENDENCIES = ["climate"]

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Daikin200Climate),
})


def to_code(config):
    var = cg.new_Pvariable(config[cv.ID])
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)
    