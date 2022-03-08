from lemon import core
from lemon.core import math
from lemon.core import string_id
from lemon.core import logger
from lemon.window import WindowSize
from lemon.events import handler
from lemon.events import event

#def subscribe(eventName):
#	def wrapper(func):
#		handler.subscribe(string_id(eventName), func)
#		return func
#	return wrapper

def test_lemon():
	w = string_id('test')
	logger.message('hello from python {}'.format(w.str))
