import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

print("\n\n🔥 DAIKIN200 INIT LOADED - OK 🔥\n\n")

DEPENDENCIES = ["climate"]
AUTO_LOAD = ["climate"]

daikin200_ns = cg.esphome_ns.namespace("daikin200")

Daikin200Climate = daikin200_ns.class_(
    "Daikin200Climate",
    climate.Climate,
    cg.Component,
)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Daikin200Climate),
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[cv.ID])
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)