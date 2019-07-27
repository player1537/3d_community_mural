#!/usr/bin/env python3.7
"""

"""

from __future__ import annotations
from http.server import HTTPServer, SimpleHTTPRequestHandler
from socketserver import ThreadingMixIn
from subprocess import Popen, PIPE
from dataclasses import dataclass
from io import BytesIO
from threading import Lock
from math import sqrt
from pathlib import Path
from typing import NewType
from PIL import Image


_g_subprocess: Subprocess = None
_g_materials: Materials = None


MaterialID = NewType('MaterialID', int)
MaterialName = NewType('MaterialName', str)
MaterialRow = NewType('MaterialRow', str)


@dataclass
class Materials:
	by_index: List[MaterialRow]
	by_name: Dict[MaterialName, MaterialID]

	@classmethod
	def from_file(cls, path: Path) -> Materials:
		by_index = []
		by_name = {}

		with open(path, 'r') as f:
			it = (x.rstrip() for x in f)
			for i, line in enumerate(it):
				name = line.split('\t')[0]

				by_index.append(line)
				by_name[name] = i
		
		return cls(by_index, by_name)
	
	@property
	def env(self) -> Dict[str, str]:
		env = {}
		for i, line in enumerate(self.by_index):
			env[f'mat_{i}'] = line
		env['nmat'] = f'{i}'
		return env
	
	def lookup(self, name) -> MaterialID:
		return self.by_name[name]


@dataclass
class Subprocess:
	process: Popen
	lock: Lock
	
	@classmethod
	def create(cls, exe, *, env=None):
		# In order to read from the subprocess, it seems like
		# we need to read from either stdout or stderr, and
		# can't read from an arbitrary file descriptor. One
		# way around this is to take the file descriptor we
		# want and turn it into stdout, and then save the old
		# stdout to stderr. We can't do that without the shell.
		process = Popen(['bash', '-c', f'{exe} 100>&1 1>&2'], stdin=PIPE, stdout=PIPE, env=env)
		lock = Lock()
		return cls(process, lock)
	
	def submit(self, request: bytes):
		self.process.stdin.write(request + b'\n')
		self.process.stdin.flush()
	
	def receive(self) -> bytes:
		data = b''
		while True:
			data += self.process.stdout.read(1)
			if b':' in data:
				break
		
		length, rest = data.split(b':')
		length = int(length)
		
		data = self.process.stdout.read(length - len(rest))
		assert len(rest) + len(data) == length, f'{len(rest)}, {len(data)}, {length}'
		comma = self.process.stdout.read(1)  # comma
		assert comma == b',', f'Comma is {comma!r}'
		return rest + data


class RequestHandler(SimpleHTTPRequestHandler):
	protocol_version = 'HTTP/1.1'

	def do_GET(self):
		if self.path == '/':
			self.directory = 'static'
			super().do_GET()
		elif self.path == '/favicon.ico':
			super().do_GET()
		elif self.path.startswith('/static/'):
			super().do_GET()
		elif self.path.startswith('/image/'):
			self.do_GET_image()
		else:
			raise NotImplementedError
	
	def do_GET_image(self):
		_, image, what, x, y, z, ux, uy, uz, vx, vy, vz, quality, optstring = self.path.split('/')
		assert _ == '', f'{_!r} is not empty'
		assert image == 'image'
		x, y, z = map(float, (x, y, z))
		ux, uy, uz = map(float, (ux, uy, uz))
		vx, vy, vz = map(float, (vx, vy, vz))
		quality = int(quality)
		
		bx = by = bz = None
		
		it = iter(what.split(','))
		type = next(it)
		if type == 'scene':
			for k in it:
				if k == 'ball':
					bx = float(next(it))
					by = float(next(it))
					bz = float(next(it))
					bsx = float(next(it))
					bsy = float(next(it))
					bsz = float(next(it))
					matid = _g_materials.lookup(next(it))
				else:
					print('bad scene type {k!r}')
					raise NotImplementedError
		else:
			print(f'bad type {scene!r}')
			raise NotImplementedError

		options = {}
		it = iter(optstring.split(','))
		for k, v in zip(it, it):
			options[k] = v

		tiling = options.get('tiling', '0-1')
		tile_index, num_tiles = tiling.split('-')
		tile_index = int(tile_index)
		num_tiles = int(num_tiles)
		n_cols = int(sqrt(num_tiles))
		assert bx is not None and by is not None and bz is not None, f'{bx!r} {by!r} {bz!r}'
		
		query = b'%f %f %f %f %f %f %f %f %f %d %f %f %f %f %f %f %d' % (
			x, y, z, ux, uy, uz, vx, vy, vz, quality, bx, by, bz, bsx, bsy, bsz, matid,
		)
		
		with _g_subprocess.lock:
			_g_subprocess.submit(query)
			data = _g_subprocess.receive()
		
		if b'error' in data:
			print(data)
		
		image = Image.frombytes('RGBX', (quality, quality), data, 'raw', 'RGBX', 0, -1)
		buffer = BytesIO()
		image.save(buffer, 'JPEG')
		
		content = buffer.getvalue()

		self.send('image/jpeg', content)
	
	def send(self, content_type, content):
		connection = self.headers['connection']
		keep_alive = False
		if connection == 'keep-alive':
			keep_alive = True
		
		self.send_response(200)
		self.send_header('Content-Type', content_type)
		self.send_header('Content-Length', str(len(content)))
		self.send_header('Access-Control-Allow-Origin', self.headers['origin'])
		if keep_alive:
			self.send_header('Connection', 'keep-alive')
		self.end_headers()
		self.wfile.write(content)


class ThreadingHTTPServer(ThreadingMixIn, HTTPServer):
	pass


def main(bind, port, exe, materials):
	env = materials.env

	subprocess = Subprocess.create(exe, env=env)
	
	global _g_subprocess
	_g_subprocess = subprocess
	
	global _g_materials
	_g_materials = materials
	
	address = (bind, port)
	print(f'Listening on {address!r}')
	server = ThreadingHTTPServer(address, RequestHandler)
	server.serve_forever()


def cli():
	def materials(s):
		return Materials.from_file(Path(s))

	import argparse

	parser = argparse.ArgumentParser()
	parser.add_argument('--port', type=int, default=8801)
	parser.add_argument('--bind', default='')
	parser.add_argument('--materials', type=materials)
	parser.add_argument('exe')
	args = vars(parser.parse_args())

	main(**args)


if __name__ == '__main__':
	cli()
