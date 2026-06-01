print("\n\n🔥 DAIKIN200 INIT LOADED - 1🔥\n\n")
import esphome.codegen as cg

daikin200_ns = cg.esphome_ns.namespace("daikin200")

Daikin200Climate = daikin200_ns.class_(
    "Daikin200Climate",
    None,
)