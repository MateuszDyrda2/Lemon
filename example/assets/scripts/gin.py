from core import *
from events import *
from input import input
from input.key import keycode
from core.math import *
from scene import *
from transform import Transform

class gin(scriptable_entity):
	def __init__(self, ent):
		scriptable_entity.__init__(self, ent)
		logger.warn('entity constructor')
	def create(self):
		logger.warn('create')
		self.transform = self.get_component('transform');
		self.rigidbody = self.get_component('rigidbody');
	def on_enable(self):
		logger.warn('name = {}'.format(self.name))
	def update(self, delta):
		if input.key_pressed(keycode.d):
			self.rigidbody.velocity = vec2(80, self.rigidbody.velocity.y)
		if input.key_pressed(keycode.a):
			self.rigidbody.velocity = vec2(-80, self.rigidbody.velocity.y)
		
	def late_update(self, delta):
		pass
	def on_disable(self):
		pass
	def destroy(self):
		logger.warn('destroy')
