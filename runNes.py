import ctypes
from ctypes import cdll
import numpy as np
import pygame
import sys
import time

A = 0
B = 2
SELECT = 6
START = 7
UP = 13
DOWN = 14
LEFT = 11
RIGHT = 12

buttons = [0, 2, 6, 7, 13, 14, 11, 12]

pygame.init()
pygame.joystick.init()

screen = pygame.display.set_mode((240, 256))

lib = cdll.LoadLibrary('nes/lib/libnes.so')
nes = lib.createEmulator()
lib.loadROM(nes, bytes('nes/roms/smb.nes', 'ascii'))

arrType = ctypes.c_uint32*(256*240)

getFrame = lib.emulateNextFrame;
arrType = ctypes.c_uint32*(256*240)
getFrame.restype = ctypes.POINTER(arrType)


i = 0;
while True:
	
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			sys.exit()
		elif event.type == pygame.KEYDOWN:
			if event.key == pygame.K_s:
				print('Save state saveState.st')
				lib.saveState(nes, bytes('saveState.st', 'ascii'))
				sys.exit()
			elif event.key == pygame.K_l:
				print('Load state saveState.st')
				lib.loadState(nes, bytes('saveState.st', 'ascii'))
			elif event.key == pygame.K_q:
				sys.exit()
	
	joystick = pygame.joystick.Joystick(0)
	time.sleep(0.01)
	if joystick is not None:
		joystick.init()
		
		state = 0;
		for i, button in enumerate(buttons):
			state |= joystick.get_button(button) << i
		
		lib.setController1State(nes, state)
	
	frame = getFrame(nes)
	addr = ctypes.addressof(frame.contents)
	arr = np.frombuffer(arrType.from_address(addr), dtype='uint32')
	
	img = pygame.image.frombuffer(arr.tostring(), (256,240), 'RGBX')
	screen.blit(img, (0,0))
	pygame.display.flip()
