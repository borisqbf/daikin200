
import esphome.codegen as cg
from esphome.components import climate

daikin200_ns = cg.esphome_ns.namespace("daikin200")

Daikin200Climate = daikin200_ns.class_(
    "Daikin200Climate",
    climate.Climate,
    cg.Component,
)