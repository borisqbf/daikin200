print("\n\n🔥 DAIKIN200 INIT LOADED - 1 🔥\n\n")

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

CODEOWNERS = ["@borisqbf"]

daikin200_ns = cg.esphome_ns.namespace("daikin200")

Daikin200Climate = daikin200_ns.class_(
    "Daikin200Climate",
    climate.Climate,
    cg.Component,
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Daikin200Climate),
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[cv.ID])
    cg.add(var)
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)

IS_PLATFORM = True