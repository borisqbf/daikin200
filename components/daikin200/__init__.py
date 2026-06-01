print("\n\n🔥 DAIKIN200 INIT LOADED - 0🔥\n\n")

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

daikin200_ns = cg.global_ns.namespace("daikin200")

daikin200_ns = cg.esphome_ns.namespace("daikin200")

Daikin200Climate = daikin200_ns.class_(
    "Daikin200Climate",
    climate.Climate,
    cg.Component,
)