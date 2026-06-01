import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

print("CLIMATE ATTRS:")
print([x for x in dir(climate) if "SCHEMA" in x])

from esphome.const import CONF_ID, CONF_NAME

from . import Daikin200Climate

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Daikin200Climate),
    cv.Optional(CONF_NAME): cv.string,
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var)
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)