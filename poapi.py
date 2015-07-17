#!/usr/bin/env python
"""
	Copyright (C) 2015 Wilbert van Ham, Stichting Katholieke Universiteit, 
	KVK 41055629, Nijmegen

	This program is free software; you can redistribute it and/or modify 
	it under the terms of the GNU General Public License as published by 
	the Free Software Foundation; either version 3 of the License, or (at 
	your option) any later version.

	This program is distributed in the hope that it will be useful, but 
	WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
	General Public License for more details.

	You should have received a copy of the GNU General Public License 
	along with this program; if not, see <http://www.gnu.org/licenses>.

	Additional permission under GNU GPL version 3 section 7

	If you modify this Program, or any covered work, by linking or 
	combining it with NDI Optotrak API library (or a modified version 
	of that library), containing parts covered by the terms of its 
	proprietary license, the licensors of this Program grant you 
	additional permission to convey the resulting work. 
"""
import os, subprocess

class Poapi(object):
	def __init__(self):
		deamon = "{}/poapi".format(os.path.dirname(os.path.realpath(__file__)))
		self.process = subprocess.Popen((deamon,), stdin=subprocess.PIPE, stdout=subprocess.PIPE)
	def __getattr__(self, methodName):
		def get(self, *args):
			s = "{:s}({})\n".format(methodName, ",".join([str(a) for a in args]))
			self.process.stdin.write(s)
			r = self.process.stdout.readline()
			if r.startswith("ERROR: no such command"):
				raise AttributeError, r
			return eval(r.rstrip(), {'__builtins__': None})
		return get.__get__(self)
