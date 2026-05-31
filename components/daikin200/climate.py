import esphome.config_validation as cv
import esphome.codegen as cg

from esphome.components import climate
from esphome.const import CONF_ID

# Link to C++ namespace
daikin200_ns = cg.esphome_ns.namespace("daikin200")
Daikin200Climate = daikin200_ns.class_(
    "Daikin200Climate",
    climate.Climate,
    cg.Component,
)

# -----------------------------
# CONFIG SCHEMA
# -----------------------------
CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Daikin200Climate),
})


# -----------------------------
# CODE GENERATION
# -----------------------------
async def to_code(config):

    var = cg.new_Pvariable(config[CONF_ID])

    # register as climate device
    await climate.register_climate(var, config)

    # register as ESPHome component
    await cg.register_component(var, config)
    