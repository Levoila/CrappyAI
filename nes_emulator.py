import ctypes
import numpy as np

class NesButtons:
	A = 1
	B = 2
	SELECT = 4
	START = 8
	UP = 16
	DOWN = 32
	LEFT = 64
	RIGHT = 128
	

class NesEmulator:
	width = 256
	height = 240
	
	def __init__(self):
		self.nesPtr = _lib.createEmulator()
		
	def loadRom(self, filename):
		_lib.loadROM(self.nesPtr, bytes(filename, 'ascii'))
		
	def loadState(self, filename):
		_lib.loadState(self.nesPtr, bytes(filename, 'ascii'))
		
	def saveState(self, filename):
		_lib.saveState(self.nesPtr, bytes(filename, 'ascii'))
		
	def getNextFrame(self):
		frame = _emulateNextFrameFunc(self.nesPtr)
		addr = ctypes.addressof(frame.contents)
		return np.frombuffer(_frameType.from_address(addr), dtype='uint32')
		
	def setController1State(self, state):
		_lib.setController1State(self.nesPtr, state)
		
_libLocation = 'nes/lib/libnes.so'
_lib = ctypes.cdll.LoadLibrary(_libLocation)
_emulateNextFrameFunc = _lib.emulateNextFrame
_frameType = ctypes.c_uint32*(NesEmulator.width*NesEmulator.height)
_emulateNextFrameFunc.restype = ctypes.POINTER(_frameType)
		
	
	
