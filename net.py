from __future__ import division
from convnet import ConvPoolLayer, HiddenLayer, relu
import theano
import theano.tensor as T
import numpy as np
import math
import sys

class NetStruct:
	def __init__(self, convLayers, fullyConnectedLayers, imgShape, rng, args):
		self.rng = rng
		self.input = T.tensor4()
		self.convLayers = []
		self.fullyConnectedLayers = []
		
		if len(imgShape) == 2:
			imgShape = (1,) + imgShape
		
		lastImgShape = imgShape
		lastPoolingShape = (1,1)
		lastFilterShape = (0,0,1,1)
		lastFilterNb = args.framesPerState
		for convLayer in convLayers:
			filterShape = convLayer[0]
			poolSize = convLayer[1]
			nbFilters = convLayer[2]
			input = self.input if len(self.convLayers) == 0 else self.convLayers[-1].output
			imageShape = (lastFilterNb,
						  int(math.ceil((lastImgShape[1] - lastFilterShape[2] + 1) / lastPoolingShape[0])),
						  int(math.ceil((lastImgShape[2] - lastFilterShape[3] + 1) / lastPoolingShape[1])))
			filterShape = (nbFilters, lastFilterNb) + filterShape
			
			conv = ConvPoolLayer(
				rng,
				input=input,
				image_shape=(None,)+imageShape,
				filter_shape=filterShape,
				stride=poolSize
			)
			
			lastImgShape = imageShape
			lastPoolingShape = poolSize
			lastFilterShape = filterShape
			lastFilterNb = nbFilters
			
			self.convLayers.append(conv)
			
		convOut = self.convLayers[-1].output.flatten(2)
			
		#TODO : This assumes no pooling/stride from the last conv layer
		for fcl in fullyConnectedLayers:
			input = convOut if len(self.fullyConnectedLayers) == 0 else self.fullyConnectedLayers[-1].output
			n_in = (lastFilterShape[0] * (lastImgShape[1] - lastFilterShape[2] + 1) * (lastImgShape[2] - lastFilterShape[3] + 1)) if len(self.fullyConnectedLayers) == 0 else self.fullyConnectedLayers[-1].n_out
			n_out = fcl
			activation = None if fcl == fullyConnectedLayers[-1] else relu
			
			layer = HiddenLayer(
				rng,
				input=input,
				n_in=n_in,
				n_out=n_out,
				activation=activation
			)
			
			self.fullyConnectedLayers.append(layer)
			
			
		self.output = self.fullyConnectedLayers[-1].output / 255.0
		self.forward = theano.function([self.input], self.output)
		
	def setParams(self, net):
		for i in range(len(self.convLayers)):
			self.convLayers[i].W.set_value(net.convLayers[i].W.get_value())
			self.convLayers[i].b.set_value(net.convLayers[i].b.get_value())
			
		for i in range(len(self.fullyConnectedLayers)):
			self.fullyConnectedLayers[i].W.set_value(net.fullyConnectedLayers[i].W.get_value())
			self.fullyConnectedLayers[i].b.set_value(net.fullyConnectedLayers[i].b.get_value())
		
	def getParams(self):
		params = []
		for i in range(len(self.convLayers)):
			params += self.convLayers[i].params
			
		for i in range(len(self.fullyConnectedLayers)):
			params += self.fullyConnectedLayers[i].params
			
		return params

class Net:
	def __init__(self, args):
		reward = T.col('r')
		action = T.icol('a')
		terminal = T.icol('t')
		discount = T.scalar('gamma')
		learningRate = T.scalar('lr')
		rho = T.scalar('rho')
		epsilon = T.scalar('eps')
		rng = np.random.RandomState(42)
		
		self.batchNb = args.batchSize
		
		#convLayers = [[(8,8),(4,4),64],
		#			  [(4,4),(2,2),128],
		#			  [(3,3),(1,1),256],
		#			  [(3,3),(1,1),512]]
		#fcl = [1024, 6]
		
		convLayers = [[(8,8),(4,4),64],
					  [(4,4),(2,2),128],
					  [(3,3),(1,1),256],
					  [(3,3),(1,1),256]]
		fcl = [1024, args.actionNb]
		self.q1 = NetStruct(convLayers, fcl, (4,100,100), rng, args)
		self.q2 = NetStruct(convLayers, fcl, (4,100,100), rng, args)
		self.q2.setParams(self.q1)
		
		self.states = theano.shared(np.zeros((args.batchSize,4,100,100), dtype='float32'))
		self.states2 = theano.shared(np.zeros((args.batchSize,4,100,100), dtype='float32'))
		self.actions = theano.shared(np.zeros((args.batchSize,1), dtype='int32'), broadcastable=(False,True))
		self.rewards = theano.shared(np.zeros((args.batchSize,1), dtype='float32'), broadcastable=(False,True))
		self.terminals = theano.shared(np.zeros((args.batchSize,1), dtype='int32'), broadcastable=(False,True))
		
		self.learningRate = theano.shared(np.array(args.learningRate, dtype='float32'))
		self.rho = theano.shared(np.array(args.rmsPropRho, dtype='float32'))
		self.epsilon = theano.shared(np.array(args.rmsPropEpsilon, dtype='float32'))
		self.discount = theano.shared(np.array(args.discountFactor, dtype='float32'))
		
		loss = self.QLoss(self.q1.output, self.q2.output, action, reward, terminal, discount)
		
		params = self.q1.getParams()
		
		updates = self.rmsProp(loss, params, rho, epsilon, learningRate)
		self.train_model = theano.function(
			[],
			loss,
			updates=updates,
			givens = { 
					   self.q1.input: self.states,
					   self.q2.input: self.states2,
					   action: self.actions,
					   reward: self.rewards,
					   terminal: self.terminals,
					   discount: self.discount,
					   learningRate: self.learningRate,
					   rho: self.rho,
					   epsilon: self.epsilon
					 }
		)
		
	def QLoss(self, q1, q2, a, r, t, discount):
		y = r + (T.ones_like(t) - t) * discount * T.max(q2, axis=1, keepdims=True)
		diff = y - q1[T.arange(self.batchNb), a.reshape((-1,))].reshape((-1,1))
		loss = diff**2
		return T.sum(loss)
		
	def learn(self, states, states2, actions, rewards, terminals):
		self.states.set_value(states)
		self.states2.set_value(states2)
		self.actions.set_value(actions)
		self.rewards.set_value(rewards)
		self.terminals.set_value(terminals)
		
		return self.train_model()
		
	def updateQ2(self):
		self.q2.setParams(self.q1)
		
	def forward(self, states):
		return self.q1.forward(states)
		
	def rmsProp(self, loss, params, rho, epsilon, learning_rate):
		grads = theano.grad(cost=loss, wrt=params)
		updates = []
		for param, grad in zip(params, grads):
			value = param.get_value(borrow=True)

			accGrad = theano.shared(np.zeros(value.shape, dtype='float32'), broadcastable=param.broadcastable)
			accGradNew = rho * accGrad + (1 - rho) * grad

			accRms = theano.shared(np.zeros(value.shape, dtype='float32'), broadcastable=param.broadcastable)
			accRmsNew = rho * accRms + (1 - rho) * grad ** 2
			
			updates.append((accGrad, accGradNew))
			updates.append((accRms, accRmsNew))
			updates.append((param, (param - learning_rate * (grad / T.sqrt(accRmsNew - accGradNew**2 + epsilon)))))
			
		return updates
		
		
		
		
		
		
		
