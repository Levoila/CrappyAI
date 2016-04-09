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
		_lib.loadROM(self.nesPtr, filename)
		
	def loadState(self, filename):
		_lib.loadState(self.nesPtr, filename)
		
	def saveState(self, filename):
		_lib.saveState(self.nesPtr, filename)
		
	def getNextFrame(self):
		frame = _emulateNextFrameFunc(self.nesPtr)
		addr = ctypes.addressof(frame.contents)
		return np.frombuffer(_frameType.from_address(addr), dtype='uint8')
		
	def setController1State(self, state):
		_lib.setController1State(self.nesPtr, state)
		
	def findValue(self, value):
		_lib.findValue8(self.nesPtr, value)
		
	def readMem16(self, addr):
		return _lib.readMem16(self.nesPtr, addr)
		
	def readMem8(self, addr):
		return _lib.readMem8(self.nesPtr, addr)
		
_libLocation = 'nes/lib/libnes.so'
_lib = ctypes.cdll.LoadLibrary(_libLocation)
_emulateNextFrameFunc = _lib.emulateNextFrame
_frameType = ctypes.c_uint8*(NesEmulator.width*NesEmulator.height*4)
_emulateNextFrameFunc.restype = ctypes.POINTER(_frameType)
		
	
	
