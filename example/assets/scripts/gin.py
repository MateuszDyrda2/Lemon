from core import *
from events import *
from input import input
from input.key import keycode
from core.math import *
from scene import *

class gin(scriptable_entity):
	def __init__(self, ent):
		scriptable_entity.__init__(self, ent)
		logger.warn('entity constructor')
	def create(self):
		logger.warn('create')
	def on_enable(self):
		pass
	def update(self, delta):
		if input.key_pressed(keycode.w):
			self.translate(vec3(0.0, 1.0, 0.0))
	def late_update(self, delta):
		pass
	def on_disable(self):
		pass
	def destroy(self):
		logger.warn('destroy')
