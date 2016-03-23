
import pygame
import sys
import time
from nes_emulator import NesEmulator

pygame.init()
pygame.joystick.init()

screen = pygame.display.set_mode((240, 256))

nes = NesEmulator()
nes.loadRom('nes/roms/smb.nes')

while True:	
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			sys.exit()
		elif event.type == pygame.KEYDOWN:
			if event.key == pygame.K_s:
				print('Save state saveState.st')
				nes.saveState('saveState.st')
			elif event.key == pygame.K_l:
				print('Load state saveState.st')
				nes.loadState('saveState.st')
			elif event.key == pygame.K_q:
				sys.exit()
	
	joystick = pygame.joystick.Joystick(0)
	time.sleep(0.02)
	if joystick is not None:
		joystick.init()
		state = 0;
		for i, button in enumerate([0, 2, 6, 7, 13, 14, 11, 12]):
			state |= joystick.get_button(button) << i
		nes.setController1State(state)
		
	frame = nes.getNextFrame()
	
	img = pygame.image.frombuffer(frame.tostring(), (nes.width,nes.height), 'RGBX')
	screen.blit(img, (0,0))
	pygame.display.flip()
