from lemon import core
from lemon.core import math

def test_lemon():
	w = core.string_id('test')
	core.logger.message('hello from python {}'.format(w.str))
	position = math.vec3(5.0, 10.0, 1.0)
	core.logger.message('position: {}.{}.{}'.format(position.x, position.y, position.z))

	