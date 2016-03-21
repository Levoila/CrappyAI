import ctypes
from ctypes import cdll
import numpy as np
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import pygame
import sys

pygame.init()
screen = pygame.display.set_mode((240, 256))

lib = cdll.LoadLibrary('./libnes.so')
nes = lib.createEmulator()
lib.loadROM(nes, 'smb.nes')

arrType = ctypes.c_uint32*(256*240)

getFrame = lib.emulateNextFrame;
arrType = ctypes.c_uint32*(256*240)
getFrame.restype = ctypes.POINTER(arrType)


i = 0;
while True:
	
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			sys.exit()
	
	
	frame = getFrame(nes)
	addr = ctypes.addressof(frame.contents)
	arr = np.frombuffer(arrType.from_address(addr), dtype='uint32')
	
	img = pygame.image.frombuffer(arr.tostring(), (256,240), 'RGBX')
	screen.blit(img, (0,0))
	pygame.display.flip()
