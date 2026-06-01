import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.components import remote_base

from esphome.const import CONF_ID, CONF_NAME, CONF_REMOTE_TRANSMITTER


from . import Daikin200Climate

CONFIG_SCHEMA = climate._CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Daikin200Climate),
    cv.Required(CONF_REMOTE_TRANSMITTER): cv.use_id(remote_base.RemoteTransmitterComponent),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    tx = yield cg.get_variable(config[CONF_REMOTE_TRANSMITTER])
    cg.add(var.set_transmitter(tx))

    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)