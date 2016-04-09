from nes_emulator import NesEmulator, NesButtons
from learning_agent import LearningAgent
import scipy
import numpy as np
import sys
import time

try:
	import pygame
except ImportError:
	print 'Pygame is not installed. Graphics disabled.' 

class Experiment:
	def __init__(self, args):
		self.epochLength = args.epochLength
		self.epochNumber = args.epochNumber
		self.testLength = args.testLength
		self.initialState = args.initialState
		self.game = args.game
		self.graphics = args.graphics
		self.visible = args.visible
		self.filename = args.filename
		self.repeatAction = args.repeatAction
		self.mergedFrames = args.mergedFrames
		self.testOnly = args.testOnly
		
		self.agent = LearningAgent(args)
		self.emulator = NesEmulator()
		self.emulator.loadRom('nes/roms/arkanoid.nes')
		self.emulator.getNextFrame()
		self.actions = args.actions
		
		if self.game == 'space_invaders':
			from space_invaders import GameAdapter
		elif self.game == 'arkanoid':
			from arkanoid import GameAdapter
		else:
			print 'UNKNOWN GAME', self.game
			
		self.gameAdapter = GameAdapter(self.emulator)
		
		self.agent.load(self.filename + '.pkl')
		
		if self.graphics:
			self.initGraphics()
			
	def runDemonstration(self):
		self.agent.beginTest()
		
		while True:
			self.emulator.loadState(self.initialState)
			frame = self.preprocess(self.emulator.getNextFrame().reshape((240,256,4)))
			action = self.agent.begin(frame)
			self.gameAdapter.reset()
			
			start = time.time()
			while True:
				self.handleEvents()
				terminal = False
				reward = 0
				
				for i in range(4):
					self.emulator.setController1State(self.actions[action])
					if i >= 3:
						frame = np.maximum(frame, self.emulator.getNextFrame().reshape((240,256,4)))
					else:
						frame = self.emulator.getNextFrame().reshape((240,256,4))
					reward += self.gameAdapter.update()
					terminal = terminal or self.gameAdapter.lost()
					self.displayFrame(frame)
					
					end = time.time()
					toSleep = 0.01666 - (end - start)
					if toSleep > 0:
						time.sleep(toSleep)
					start = time.time()
				
				if terminal:
					self.agent.end(reward)
					break
				
				action = self.agent.tick(self.preprocess(frame), reward)
					
				
	def run(self):
		
		if self.testOnly:
			self.runDemonstration()
			return
		
		stepsLeft = 0
		for epoch in range(self.epochNumber):
			stepsLeft = max(0, self.epochLength + stepsLeft) #For when stepsLeft goes below 0
			
			print 'Running epoch ', epoch+1
			
			while stepsLeft > 0:
				steps = self.runEpisode()
				stepsLeft -= steps
				
			print 'Starting testing'
			self.agent.beginTest()
			testStepsLeft = self.testLength
			while testStepsLeft > 0:
				steps = self.runEpisode()
				testStepsLeft -= steps
			self.agent.endTest()
			
			self.agent.save(self.filename + '.pkl')
			
	def runEpisode(self):
		self.emulator.loadState(self.initialState)
		frame = self.preprocess(self.emulator.getNextFrame().reshape((240,256,4)))
		action = self.agent.begin(frame)
		self.gameAdapter.reset()
		
		steps = 0
		while True:
			self.handleEvents()
			
			steps += 1
			reward, terminal, frame = self.tick(action)
			
			if terminal:
				self.agent.end(reward)
				break
				
			action = self.agent.tick(self.preprocess(frame), reward)
			
		return steps
		
	def tick(self, action):
		reward = 0
		terminal = False
		frame = None
		for i in range(self.repeatAction):
			self.emulator.setController1State(self.actions[action])
			
			if i >= (self.repeatAction - self.mergedFrames):
				frame = np.maximum(frame, self.emulator.getNextFrame().reshape((240,256,4)))
			else:
				frame = self.emulator.getNextFrame().reshape((240,256,4))
				
			reward += self.gameAdapter.update()
			terminal = terminal or self.gameAdapter.lost()
			
			if self.graphics and self.visible:
				self.displayFrame(frame)
			
		return reward, terminal, frame
				
		
	def initGraphics(self):
		self.screen = pygame.display.set_mode((256,240))
		
	def closeWindow(self):
		pygame.display.quit()
		
	def displayFrame(self, frame):
		img = pygame.image.frombuffer(frame.tostring(), (256,240), 'RGBX')
		self.screen.blit(img, (0,0))
		pygame.display.flip()
		
	def handleEvents(self):
		if not self.graphics:
			return
		
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				sys.exit()
			elif event.type == pygame.KEYDOWN:
				if event.key == pygame.K_q:
					sys.exit()
				elif event.key == pygame.K_v:
					self.visible = not self.visible
				elif event.key == pygame.K_p:
					self.agent.printStuff()
		
	def preprocess(self, frame):
		workingFrame = frame[35:230, 5:200, :]
		workingFrame = scipy.misc.imresize(workingFrame, (100,100))
		workingFrame = np.dot(workingFrame, [0.299, 0.114, 0.587, 0.0]).astype('uint8')
		return workingFrame
