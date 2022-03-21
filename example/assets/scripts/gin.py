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
		logger.warn('name = {}'.format(self.name))
	def update(self, delta):
		if input.key_pressed(keycode.d):
			self.translate(vec2(200.0, 0.0) * delta)
		if input.key_pressed(keycode.a):
			self.translate(vec2(-200.0, 0.0) * delta)
		
	def late_update(self, delta):
		pass
	def on_disable(self):
		pass
	def destroy(self):
		logger.warn('destroy')
