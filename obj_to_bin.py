#!/usr/bin/env python3.7
"""

"""

from __future__ import annotations
from pathlib import Path
from struct import pack
from pywavefront import Wavefront


def main(filename, output, rescale):
	scene = Wavefront(filename, collect_faces=True)
	
	vertex_bin = b''
	vertices = scene.vertices
	nvertices = len(vertices)
	if rescale:
		xmin = xmax = vertices[0][0]
		ymin = ymax = vertices[0][1]
		zmin = zmax = vertices[0][2]
		for x, y, z in vertices:
			xmin = min(xmin, x)
			xmax = max(xmax, x)
			ymin = min(ymin, y)
			ymax = max(ymax, y)
			zmin = min(zmin, z)
			zmax = max(zmax, z)
		
		vmin = min(xmin, ymin, zmin)
		vmax = max(xmax, ymax, zmax)

	for x, y, z in vertices:
		if rescale:
			x = (x - xmin - (xmax - xmin) / 2) / (vmax - vmin)
			y = (y - ymin - (ymax - ymin) / 2) / (vmax - vmin)
			z = (z - zmin - (zmax - zmin) / 2) / (vmax - vmin)
		vertex_bin += pack('fff', x, y, z)
	
	index_bin = b''
	nindex = 0
	for name, mesh in scene.meshes.items():
		indices = mesh.faces
		nindex += len(indices)
		for i, j, k in indices:
			assert i < nvertices or j < nvertices or k < nvertices
			index_bin += pack('III', i, j, k)
	
	has_vertex = True
	has_index = True
	has_extra1 = False
	has_extra2 = False
	header = pack('cccc',
		b'V' if has_vertex else b' ',
		b'I' if has_index else b' ',
		b' ' if has_extra1 else b' ',
		b' ' if has_extra2 else b' ',
	)
	
	if has_vertex:
		vertex_header = pack('I', nvertices)
	
	if has_index:
		index_header = pack('I', nindex)
	
	if has_extra1:
		raise NotImplementedError
	
	if has_extra2:
		raise NotImplementedError
	
	content = header
	if has_vertex:
		content += vertex_header
		content += vertex_bin
	if has_index:
		content += index_header
		content += index_bin
	if has_extra1:
		raise NotImplementedError
	if has_extra2:
		raise NotImplementedError
	
	if output is not None:
		output.write_bytes(content)


def cli():
	import argparse

	parser = argparse.ArgumentParser()
	parser.add_argument('filename')
	parser.add_argument('--output', type=Path)
	parser.add_argument('--rescale', action='store_true')
	parser.add_argument('--no-rescale', dest='rescale', action='store_false')
	args = vars(parser.parse_args())

	main(**args)


if __name__ == '__main__':
	cli()
