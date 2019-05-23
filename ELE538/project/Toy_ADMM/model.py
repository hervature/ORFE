import tensorflow as tf
import numpy as np

class Toy_ADMM():
  """
  Toy example where the network is trying to predict
  Y = AX + B
  where X ~ N(0, Sigma)

  ADMM version
  f(x) is MSE of network
  g(z) is regularizer
  """

  def __init__(self):
    self.dim_x = [25,25]
    self.dim_y = [5]

    self.rho = 0.000001

    # tensorflow setup
    self.graph = tf.Graph()
    with self.graph.as_default():

      self.is_training = tf.placeholder(tf.bool, name='training')

      self.build()

      update_ops = tf.get_collection(tf.GraphKeys.UPDATE_OPS)

      with tf.control_dependencies(update_ops):
        self.optimizer = tf.train.AdamOptimizer().minimize(self.loss_f)

      self.saver = tf.train.Saver()
      self.sess = tf.Session()
      self.init = tf.global_variables_initializer()
      self.sess.run(self.init)

  def train(self, x, y):
    data = {self.x: x, self.y: y, self.is_training: True}
    self.sess.run(self.optimizer, feed_dict=data)

  def get_losses(self, x, y):
    data = {self.x: x, self.y: y, self.is_training: False}

    # Run optimization op (backprop)
    return self.sess.run([self.loss_f, self.loss_g, self.loss_total], feed_dict=data)

  def update_theta2(self):
    self.sess.run(self.update_theta2_op)

  def update_lambda(self):
    self.sess.run(self.update_lambda_op)

  def set_params(self, path):
    if tf.train.checkpoint_exists(path):
      self.saver.restore(self.sess, path)
    else:
      # if no network, save the current initialized network
      self.saver.save(self.sess, path)

  def get_variables(self):
    return self.sess.run([self.diff_vec, self.l2_norm, self.inner_product, self.lambd])

  def save_params(self, path):
    self.saver.save(self.sess, path)

  def build(self):
    # inputs and labels of network
    self.x = tf.placeholder(tf.float32, [None] + self.dim_x, name='input')
    self.y = tf.placeholder(tf.float32, [None] + self.dim_y, name='output')

    self.pred_y = self.network(self.x)

    # vectorize the parameters
    self.theta1 = tf.concat([tf.reshape(var, [-1]) for var in tf.trainable_variables()], axis=0)
    self.theta2 = tf.Variable(tf.zeros_like(self.theta1), name="theta2", trainable=False)
    self.lambd = tf.Variable(tf.zeros_like(self.theta1), name="lambda", trainable=False)

    self.update_theta2_op = tf.assign(self.theta2, tf.scalar_mul(1/(1+self.rho), tf.subtract(self.theta1, self.lambd)))
    self.update_lambda_op = tf.assign(self.lambd, tf.add(self.lambd, tf.scalar_mul(self.rho, tf.subtract(self.theta1, self.theta2))))

    self.diff_vec = tf.subtract(self.theta1, self.theta2)
    self.l2_norm = tf.nn.l2_loss(self.diff_vec)
    self.inner_product = tf.reduce_sum(tf.multiply(self.lambd, self.diff_vec))

    self.loss_f = tf.losses.mean_squared_error(self.y, self.pred_y) + self.inner_product + self.rho*self.l2_norm
    self.loss_g = tf.nn.l2_loss(self.theta2) + self.inner_product + self.rho*self.l2_norm

    self.loss_regularizer = tf.nn.l2_loss(self.theta1)
    self.loss_total = self.loss_f + self.loss_regularizer

  def network(self, x):

    with tf.variable_scope("network") as scope:
      batch_size = tf.shape(x)[0]
      x = tf.reshape(x, [batch_size] + self.dim_x + [1])

      # Layer 1 - [batch_size,50,50,1] in [batch_size,50,50,256] out
      filters = 256
      kernel_size = [3,3]

      conv = tf.layers.conv2d(x, filters, kernel_size, padding='same')
      batch_norm = tf.contrib.layers.batch_norm(conv, decay=0.9, is_training=self.is_training)
      relu = tf.nn.relu(batch_norm)

      # Layer 2 - [batch_size,50,50,256] in [batch_size,50,50,128] out
      filters = 128
      kernel_size = [3,3]

      conv = tf.layers.conv2d(x, filters, kernel_size, padding='same')
      batch_norm = tf.contrib.layers.batch_norm(conv, decay=0.9, is_training=self.is_training)
      relu = tf.nn.relu(batch_norm)

      # Layer 3 - Fully connected [batch_size,50,50,128] in [batch_size,50,1] out
      # flatten first
      relu = tf.contrib.layers.flatten(relu)

      # Fully connected layer to output
      full = tf.contrib.layers.fully_connected(relu, self.dim_y[0], activation_fn=None)
      full = tf.contrib.layers.dropout(full, is_training=self.is_training)

      return full