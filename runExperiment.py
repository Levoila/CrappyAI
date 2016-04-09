from experiment import Experiment
from nes_emulator import NesButtons

class Args:
	epochLength = 250000
	epochNumber = 20
	testLength = 50000
	initialState = 'arkanoid.st'
	game = 'arkanoid'
	graphics = True
	visible = True
	testOnly = False
	filename = 'out'
	repeatAction = 4
	mergedFrames = 2
	epsilonStart = 1.0
	epsilonEnd = 0.1
	epsilonDecayLength = 1000000
	testEpsilon = 0.05
	replaySize = 1000000
	minReplaySize = 50000
	framesPerState = 4
	learnFrequency = 4
	targetNetworkUpdateFrequency = 10000
	batchSize = 32
	learningRate = 0.00025
	discountFactor = 0.99
	rmsPropRho = 0.95
	rmsPropEpsilon = 0.01
	actions = [
			0,
			NesButtons.A,
			NesButtons.LEFT,
			NesButtons.RIGHT,
			NesButtons.LEFT | NesButtons.A,
			NesButtons.RIGHT | NesButtons.A,
		  ]
	actionNb = 6
	
def main():
	experiment = Experiment(Args)
	experiment.run()
	
if __name__ == '__main__':
	main()
		
		
		
		
		
		
		
