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
from PIL import Image


_g_subprocess: Subprocess = None


@dataclass
class Subprocess:
	process: Popen
	lock: Lock
	
	@classmethod
	def create(cls, exe):
		process = Popen(['bash', '-c', f'{exe} 100>&1 1>&2'], stdin=PIPE, stdout=PIPE)
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

		options = {}
		it = iter(optstring.split(','))
		for k, v in zip(it, it):
			options[k] = v

		tiling = options.get('tiling', '0-1')
		tile_index, num_tiles = tiling.split('-')
		tile_index = int(tile_index)
		num_tiles = int(num_tiles)
		n_cols = int(sqrt(num_tiles))
		
		query = b'%f %f %f %f %f %f %f %f %f %d' % (
			x, y, z, ux, uy, uz, vx, vy, vz, quality,
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


def main(bind, port, exe):
	subprocess = Subprocess.create(exe)
	
	global _g_subprocess
	_g_subprocess = subprocess
	
	address = (bind, port)
	print(f'Listening on {address!r}')
	server = ThreadingHTTPServer(address, RequestHandler)
	server.serve_forever()


def cli():
	import argparse

	parser = argparse.ArgumentParser()
	parser.add_argument('--port', type=int, default=8860)
	parser.add_argument('--bind', default='')
	parser.add_argument('exe')
	args = vars(parser.parse_args())

	main(**args)


if __name__ == '__main__':
	cli()
