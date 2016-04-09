import numpy as np
from data import Data
from net import Net
import cPickle
import os.path
import collections

import sys
sys.setrecursionlimit(10000)

class LearningAgent:
	def __init__(self, args):
		self.epsilonStart = args.epsilonStart
		self.epsilonEnd = args.epsilonEnd
		self.epsilonDecayLength = args.epsilonDecayLength
		self.testEpsilon = args.testEpsilon
		self.replaySize = args.replaySize
		self.minReplaySize = args.minReplaySize
		self.framesPerState = args.framesPerState
		self.learnFrequency = args.learnFrequency
		self.targetNetworkUpdateFrequency = args.targetNetworkUpdateFrequency
		self.batchSize = args.batchSize
		
		self.actionNb = args.actionNb
		
		self.lastAction = 0
		self.lastFrame = None
		self.rng = np.random.RandomState(42)
		self.data = Data(self.replaySize, self.framesPerState, (100,100))
		self.tickCount = 0
		self.learnCount = 0
		
		self.rewardAcc = 0.0
		self.episodeNb = 0
		self.qValueAcc = 0.0
		self.qValueNb = 0
		self.maxReward = 0
		self.episodeReward = 0
		self.test = False
		
		self.lastQs = collections.deque(maxlen=60)
		
		self.net = Net(args)
		self.qValues = []
		self.rewards = []
		self.tickCount = 0
			
	def load(self, filename):
		if os.path.isfile(filename):
			print 'Using', filename
			f = open(filename, 'r')
			data = cPickle.load(f)
			self.net = data['net']
			self.qValues = data['qValues']
			self.rewards = data['rewards']
			self.tickCount = data['tickCount']
			self.learnCount = data['learnCount']
			f.close()
			
	def save(self, filename):
		f = open(filename, 'w')
		data = {'net':self.net, 'qValues':self.qValues, 'rewards':self.rewards, 'tickCount':self.tickCount, 'learnCount':self.learnCount}
		cPickle.dump(data, f, -1)
		f.close()
		
	def printStuff(self):
		import matplotlib.pyplot as plt
		n = 60
		for i,v in enumerate(self.lastQs):
			print '#', i, ':', v
		frames = self.data.getLastFrames(n)
		for p in range(n):
			plt.subplot(10, 6, p+1)
			plt.imshow(frames[p,:,:], interpolation='none', cmap='gray')
		plt.show(block=False)
		
		d = []
		for i,q in enumerate(self.lastQs):
			if type(q) is not int:
				d.append(np.max(q, axis=1))
		
		plt.figure()
		plt.plot(d)
		plt.show(block=True)
		
	def beginTest(self):
		self.test = True
		self.episodeReward = 0
		self.rewardAcc = 0.0
		self.episodeNb = 0
		self.qValueAcc = 0.0
		self.qValueNb = 0
		
		
	def endTest(self):
		self.test = False
		self.qValues.append(self.qValueAcc / self.qValueNb)
		self.rewards.append(self.rewardAcc / self.episodeNb)
		
		print 'TEST :', ' qValue =', self.qValues[-1], '  reward = ', self.rewards[-1]
		
	#Inputs : results of an action (a frame and a reward)
	def tick(self, frame, reward):
		self.data.addData(self.lastFrame, self.lastAction, reward, False)
		state = self.data.getLastState(frame)
		action = self.chooseAction(state)
		
		self.episodeReward += reward
		self.rewardAcc += reward
		
		if self.data.getSize() > self.minReplaySize and self.tickCount % self.learnFrequency == 0 and not self.test:
			self.learn()
		
		self.lastFrame = frame
		self.lastAction = action
		
		self.tickCount += 1
		
		return action
			
		
	def begin(self, frame):
		self.lastFrame = frame
		self.lastAction = self.rng.randint(0, self.actionNb)
		self.episodeNb += 1
		return self.lastAction
		
	def end(self, reward):
		self.data.addData(self.lastFrame, self.lastAction, reward, True)
		if self.episodeReward > self.maxReward:
			self.maxReward = self.episodeReward
			print 'MAX REWARD :', self.maxReward
		self.episodeReward = 0
		return
		
	def chooseAction(self, state):
		epsilon = self.testEpsilon
		if not self.test:
			epsilon = self.epsilonStart - (self.epsilonStart - self.epsilonEnd) * self.tickCount / self.epsilonDecayLength
			epsilon = max(self.epsilonEnd, epsilon)
		if self.rng.rand() > epsilon:
			v = self.net.forward(state)
			self.lastQs.append(v)
			self.qValueNb += 1
			self.qValueAcc += max(max(v))
			return np.argmax(v)
		else:
			r = self.rng.randint(0, self.actionNb)
			self.lastQs.append(r)
			return r
		
	def learn(self):
		self.learnCount += 1
		states, actions, rewards, terminals, states2 = self.data.getBatch(32)
		self.net.learn(states, states2, actions, rewards, terminals)
		if self.learnCount % self.targetNetworkUpdateFrequency == 0:
			self.net.updateQ2()
			
	def showState(self, states, states2):
		import matplotlib.pyplot as plt
		for p in range(4):
			plt.subplot(2, 4, p+1)
			plt.imshow(states[0,p,:,:], interpolation='none', cmap='gray')
		for p in range(4):
			plt.subplot(2, 4, p+5)
			plt.imshow(states2[0,p,:,:], interpolation='none', cmap='gray')	
		plt.show()
