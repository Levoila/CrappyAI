class GameAdapter:
	livesAddr = 13
	blocksRemainingAddr = 15
	
	def __init__(self, nes):
		self.nes = nes;
		self.lives = nes.readMem8(GameAdapter.livesAddr)
		self.blocksRemaining = nes.readMem8(GameAdapter.blocksRemainingAddr)
		self.reward = 0
		self.l = False
		
	def update(self):
		newLives = self.nes.readMem8(GameAdapter.livesAddr)
		newBlocksRemaining = self.nes.readMem8(GameAdapter.blocksRemainingAddr)
		
		if (newLives < self.lives):
			self.reward = -1
			self.l = True
		elif (newBlocksRemaining < self.blocksRemaining):
			self.reward = 1
		else:
			self.reward = 0
		
		self.lives = newLives
		self.blocksRemaining = newBlocksRemaining
		
		return self.reward
		
	def reset(self):
		self.lives = self.nes.readMem8(GameAdapter.livesAddr)
		self.blocksRemaining = self.nes.readMem8(GameAdapter.blocksRemainingAddr)
		self.reward = 0
		self.l = False
		
	def lost(self):
		return self.l
