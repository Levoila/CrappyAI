import theano
import theano.tensor as T
from theano.tensor.nnet import conv2d
import numpy

#inspired by http://deeplearning.net/tutorial/lenet.html
# and http://deeplearning.net/tutorial/mlp.html

class ConvPoolLayer(object):
    """Pool Layer of a convolutional network """

    def __init__(self, rng, input, filter_shape, image_shape, stride=(2, 2)):

        assert image_shape[1] == filter_shape[1]
        self.input = input

        
        fan_in = numpy.prod(filter_shape[1:])
        fan_out = (filter_shape[0] * numpy.prod(filter_shape[2:]) // numpy.prod(stride))
        
        #Glorot initialization http://andyljones.tumblr.com/post/110998971763/an-explanation-of-xavier-initialization
        W_bound = numpy.sqrt(3) * numpy.sqrt(1.0 / fan_in)
        self.W = theano.shared(
            numpy.asarray(
                rng.uniform(low=-W_bound, high=W_bound, size=filter_shape),
                dtype=theano.config.floatX
            ),
            borrow=True
        )

        self.b = theano.shared(numpy.zeros((filter_shape[0],), dtype=theano.config.floatX) + 0.1, borrow=True)

        conv_out = conv2d(
            input=input,
            filters=self.W,
            filter_shape=filter_shape,
            subsample=stride,
            image_shape=image_shape
        )

        self.output = relu(conv_out + self.b.dimshuffle('x', 0, 'x', 'x'))

        self.params = [self.W, self.b]

        self.input = input
        
#From https://github.com/Theano/Theano/blob/master/theano/tensor/nnet/nnet.py#L2187
#to be comatible with theano 0.7.0
def relu(x):
	return 0.5 * (x + abs(x))
        
class HiddenLayer(object):
    def __init__(self, rng, input, n_in, n_out, W=None, b=None,
                 activation=relu):
					 
        self.n_out = n_out
        self.input = input
        
        if W is None:
			W_bound = numpy.sqrt(3) * numpy.sqrt(1.0 / n_in)
			self.W = theano.shared(numpy.asarray(rng.uniform(low=-W_bound, high=W_bound, size=(n_in, n_out)), dtype=theano.config.floatX), borrow=True)

        if b is None:
            self.b = theano.shared(numpy.zeros((n_out,), dtype=theano.config.floatX) + 0.1, name='b', borrow=True)

        lin_output = T.dot(input, self.W) + self.b
        self.output = (
            lin_output if activation is None
            else activation(lin_output)
        )
        
        self.params = [self.W, self.b]




