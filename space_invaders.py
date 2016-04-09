class GameAdapter:
	livesAddr = 41
	scoreAddr = 34
	
	def __init__(self, nes):
		self.nes = nes;
		self.lives = nes.readMem8(GameAdapter.livesAddr)
		self.score = nes.readMem16(GameAdapter.scoreAddr)
		self.reward = 0
		self.l = False
		
	def update(self):
		newLives = self.nes.readMem8(GameAdapter.livesAddr)
		newScore = self.nes.readMem16(GameAdapter.scoreAddr)
		
		if (newLives == 255):
			self.reward = -1
		elif (newLives < self.lives):
			self.reward = -1
			self.l = True
		elif (newScore > self.score):
			self.reward = 1
		else:
			self.reward = 0
		
		self.lives = newLives
		self.score = newScore
		
		return self.reward
		
	def reset(self):
		self.lives = self.nes.readMem8(GameAdapter.livesAddr)
		self.score = self.nes.readMem16(GameAdapter.scoreAddr)
		self.reward = 0
		self.l = False
		
	def lost(self):
		return self.l
