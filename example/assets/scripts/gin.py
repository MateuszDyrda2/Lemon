from lemon.core import *
from lemon.events import *
from lemon.window.key import keycode
from lemon.window import input
from lemon.core.math import *
from lemon.scene import *

def create():
	logger.warn('Script initialized')	
	logger.warn('Entity id = {}'.format(ent.id))
	#handler.subscribe('KeyPressed', move)


def update(delta):
	if(game.input().key_pressed(keycode.w)):
		ent.translate(vec3(0.0, 100.0, 0.0) * delta)
	if(game.input().key_pressed(keycode.s)):
		ent.translate(vec3(0.0, -100.0, 0.0) * delta)
	if(game.input().key_pressed(keycode.d)):
		ent.translate(vec3(100.0, 0.0, 0.0) * delta)
	if(game.input().key_pressed(keycode.a)):
		ent.translate(vec3(-100.0, 0.0, 0.0) * delta)

def late_update(delta):
	pass

def destroy():
	logger.warn('Script destroyed')