import numpy as np

class Data:
	def __init__(self, memSize, recentMemSize, frameDims):
		self.frames = np.zeros((memSize,) + frameDims, dtype='uint8')
		self.rewards = np.zeros((memSize), dtype='float32')
		self.actions = np.zeros((memSize), dtype='uint8')
		self.terminals = np.zeros((memSize), dtype='bool')
		
		self.start = 0
		self.end = 0
		self.currentSize = 0
		self.size = memSize
		self.recentMemSize = recentMemSize
		self.frameDims = frameDims
		
		self.rng = np.random.RandomState(42)
		
	#action produced by looking at frame, then gets reward and terminal
	def addData(self, frame, action, reward, terminal):
		self.actions[self.end] = action
		self.frames[self.end] = frame
		self.rewards[self.end] = reward
		self.terminals[self.end] = terminal
		
		self.end = (self.end + 1) % self.size
		if self.currentSize == self.size:
			self.start = self.end
		self.currentSize = min(self.size, self.currentSize + 1)
		
		
	def getLastState(self, frame):
		state = np.zeros((self.recentMemSize,) + self.frameDims, dtype='float32')
		indices = np.arange(self.end - self.recentMemSize + 1, self.end)
		state[0:self.recentMemSize-1] = self.frames.take(indices, axis=0, mode='wrap').astype('float32')
		state[-1] = frame.astype('float32')
		return np.expand_dims(state, axis=0)
		
	def getLastFrames(self, n):
		indices = np.arange(self.end - n, self.end)
		frames = self.frames.take(indices, axis=0, mode='wrap')
		return frames
		
	#Returns a random batch of transitions of the form state, action, reward, terminal, next_state
	def getBatch(self, size):
		states = np.zeros((size,self.recentMemSize) + self.frameDims, dtype='float32')
		states2 = np.zeros_like(states, dtype='float32')
		actions = np.zeros((size,1), dtype='uint8')
		rewards = np.zeros((size,1), dtype='float32')
		terminals = np.zeros((size,1), dtype='bool')
		
		stateCount = 0
		
		while stateCount != size:
			index = self.rng.randint(self.start + self.recentMemSize - 1, self.start + self.currentSize)
			indices = np.arange(index - self.recentMemSize + 1, index + 1)
			
			#Having a terminal frame before the end of the state if not defined for states. 
			if np.any(self.terminals.take(indices[:-1], mode='wrap')):
				continue
			
			states[stateCount] = self.frames.take(indices, axis=0, mode='wrap').astype('float32')
			states2[stateCount] = self.frames.take(indices+1, axis=0, mode='wrap').astype('float32')
			actions[stateCount] = self.actions.take(index, mode='wrap')
			rewards[stateCount] = self.rewards.take(index, mode='wrap')
			terminals[stateCount] = self.terminals.take(index, mode='wrap')
			
			stateCount += 1
			
		return states, actions, rewards, terminals, states2
		
	def getSize(self):
		return self.currentSize
		
