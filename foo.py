from PIL import Image
from pathlib import Path


image_data = Path('out.bin').read_bytes()
image = Image.frombytes('RGBA', (512, 512), image_data, 'raw', 'RGBA', 0, -1);
image.save('out.png')
