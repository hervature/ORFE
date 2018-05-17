import tensorflow as tf
import numpy as np

class DCGAN():

  def __init__(self):
    self.image_shape = [28,28,1]
    # noisy input
    self.dim_z = 100

    # optimization params
    self.learning_rate = 0.0002
    self.beta_1 = 0.5

    # tensorflow setup
    self.graph = tf.Graph()
    with self.graph.as_default():

      self.is_training = tf.placeholder(tf.bool, name='training')

      self.build()

      update_ops = tf.get_collection(tf.GraphKeys.UPDATE_OPS)

      self.g_vars = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES, "generator")
      self.d_vars = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES, "discriminator")

      with tf.control_dependencies(update_ops):
        self.g_optimizer = tf.train.AdamOptimizer(self.learning_rate, self.beta_1).minimize(self.generator_loss, var_list=self.g_vars)
        self.d_optimizer = tf.train.AdamOptimizer(self.learning_rate, self.beta_1).minimize(self.discriminator_loss, var_list=self.d_vars)

      self.saver = tf.train.Saver()
      self.sess = tf.Session()
      self.init = tf.global_variables_initializer()
      self.sess.run(self.init)

  def train_generator(self, Z):
    data = {self.Z: Z, self.image: np.empty([0,28,28,1]), self.is_training: True}
    self.sess.run(self.g_optimizer, feed_dict=data)

  def train_discriminator(self, Z, image):
    data = {self.Z: Z, self.image: image, self.is_training: True}
    self.sess.run(self.d_optimizer, feed_dict=data)

  def get_losses(self, Z, image):
    data = {self.Z: Z, self.image: image, self.is_training: True}
    return self.sess.run([self.generator_loss, self.discriminator_loss], feed_dict=data)

  def generate_images(self, Z):
    data = {self.Z: Z, self.image: np.empty([0,28,28,1]), self.is_training: True}
    return self.sess.run(self.image_gen, feed_dict=data)

  def set_params(self, path):
    if tf.train.checkpoint_exists(path):
      self.saver.restore(self.sess, path)
    else:
      # if no network, save the current initialized network
      self.saver.save(self.sess, path)

  def save_params(self, path):
    self.saver.save(self.sess, path)

  def build(self):
    # inputs and labels of network
    self.Z = tf.placeholder(tf.float32, [None, self.dim_z], name='input_noise')
    self.image = tf.placeholder(tf.float32, [None] + self.image_shape, name='input_image')

    self.image_gen = self.generator(self.Z)
    p_gen, logit_gen = self.discriminator(self.image_gen, reuse=False)
    p_real, logit_real = self.discriminator(self.image, reuse=True)
    ones = tf.ones_like(p_gen)

    self.discriminator_loss = tf.losses.sigmoid_cross_entropy(ones, logit_real) + tf.losses.sigmoid_cross_entropy(ones, (ones-logit_gen))
    self.generator_loss = tf.losses.sigmoid_cross_entropy(ones, logit_gen)

  def generator(self, Z):

    with tf.variable_scope("generator") as scope:
      batch_size = tf.shape(Z)[0]

      # Layer 1 - [None,100] in [None,4,4,512] out
      # Make into a 4-tensor by fully-connected layer, what they call projecting and reshaping
      fully_connected = tf.contrib.layers.fully_connected(Z, 4*4*512, activation_fn=None)
      fully_connected = tf.reshape(fully_connected, shape=[batch_size, 4, 4, 512])
      batch_norm = tf.contrib.layers.batch_norm(fully_connected, decay=0.9, is_training=self.is_training)
      relu = tf.nn.relu(batch_norm)

      # Layer 2 - [None,4,4,512] in [None,7,7,256] out
      filters = 256
      kernel_size = [4,4]
      strides = [1,1]
      conv = tf.layers.conv2d_transpose(relu, filters, kernel_size, strides=strides, padding='valid')
      batch_norm = tf.contrib.layers.batch_norm(conv, decay=0.9, is_training=self.is_training)
      relu = tf.nn.relu(batch_norm)

      # Layer 3 - [None,7,7,256] in [None,14,14,128] out
      filters = 128
      kernel_size = [5,5]
      strides = [2,2]
      conv = tf.layers.conv2d_transpose(relu, filters, kernel_size, strides=strides, padding='same')
      batch_norm = tf.contrib.layers.batch_norm(conv, decay=0.9, is_training=self.is_training)
      relu = tf.nn.relu(batch_norm)

      # Final layer, one channel for grey scale - [None,14,14,128] in [None,28,28,1] out
      filters = 1
      kernel_size = [5,5]
      strides = [2,2]
      conv = tf.layers.conv2d_transpose(relu, filters, kernel_size, strides=strides, padding='same')
      output = tf.tanh(conv)

      return output

  def discriminator(self, image, reuse):

    # 4 layer classifier

    with tf.variable_scope("discriminator", reuse=reuse) as scope:
      batch_size = tf.shape(image)[0]

      # convolution params
      kernel_size = [5,5]
      strides = [2,2]

      # Layer 1 - [None,28,28,1] in [None,14,14,64] out
      filters = 64
      conv = tf.layers.conv2d(image, filters, kernel_size, strides=strides, padding='same')
      batch_norm = tf.contrib.layers.batch_norm(conv, decay=0.9, is_training=self.is_training)
      relu = tf.nn.leaky_relu(batch_norm, alpha=0.2)

      # Layer 2 - [None,14,14,64] in [None,7,7,128] out
      filters = 128
      conv = tf.layers.conv2d(relu, filters, kernel_size, strides=strides, padding='same')
      batch_norm = tf.contrib.layers.batch_norm(conv, decay=0.9, is_training=self.is_training)
      relu = tf.nn.leaky_relu(batch_norm, alpha=0.2)

      # Layer 3 - [None,7,7,128] in [None,4,4,256] out
      filters = 256
      conv = tf.layers.conv2d(relu, filters, kernel_size, strides=strides, padding='same')
      batch_norm = tf.contrib.layers.batch_norm(conv, decay=0.9, is_training=self.is_training)
      relu = tf.nn.leaky_relu(batch_norm, alpha=0.2)

      # Get classifier from fully connected layer
      # Layer 4 - [None,4,4,256] in [None, 1] out
      # flatten first
      relu = tf.contrib.layers.flatten(relu)
      fully_connected = tf.contrib.layers.fully_connected(relu, 1, activation_fn=None)
      prob = tf.nn.sigmoid(fully_connected)

      return prob, fully_connected