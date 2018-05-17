import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

from model import DCGAN_ADMM

def main():
  train()

def train(num_epochs=1000, batch_size=128):

  path = "./checkpoints/model.ckpt"
  mnist = tf.contrib.learn.datasets.load_dataset("mnist")
  train_data = mnist.train.images.reshape(-1,28,28,1)

  dc_gan = DCGAN_ADMM()
  dc_gan.set_params(path)
  g_loss = []
  d1_loss = []
  d2_loss = []
  d_total_loss = []

  rho = 0.000001
  rho_max = 0.000001

  for i in range(num_epochs):
    print("=== In Epoch " + str(i) + " ===")

    # loop through whole training data in random order
    index = np.arange(len(train_data))
    np.random.shuffle(index)
    train_data = train_data[index]

    for start, end in zip(range(0, len(train_data), batch_size), range(batch_size, len(train_data), batch_size)):
      Z = np.random.uniform(-1,1, size=(batch_size, dc_gan.dim_z)).astype(np.float32)
      image = train_data[start:end].astype(np.float32)

      if i % 3 == 0:
        # first train generator
        dc_gan.train_generator(Z)
      elif i % 3 == 1:
        # second train discriminator1
        dc_gan.train_discriminator1(Z, image, rho)
      else:
        # third train discriminator2 and update lambda and rho
        dc_gan.train_discriminator2(Z, rho)
        dc_gan.update_lambda(rho)
        rho = min(rho*1.1, rho_max)

    generator_loss, discriminator1_loss, discriminator2_loss = dc_gan.get_losses(Z, image, rho)

    if i % 3 == 0 and i > 0:
      print("Discriminator 1 Loss: " + str(discriminator1_loss))
      d1_loss.append(discriminator1_loss)
    elif i % 3 == 1:
      print("Discriminator 2 Loss: " + str(discriminator2_loss))
      d2_loss.append(discriminator2_loss)
    elif i % 3 == 2:
      print("Generator Loss: " + str(generator_loss))
      g_loss.append(generator_loss)

    if i % 3 == 0:
      num_images = 64
      per_row = 8

      Z = np.random.uniform(-1,1, size=(num_images, dc_gan.dim_z)).astype(np.float32)
      image = dc_gan.generate_images(Z)
      tmp = np.empty(shape=[28*per_row,28*per_row])

      for j in range(num_images):
        x = j % per_row
        y = j // per_row

        tmp[x*28:(x+1)*28,y*28:(y+1)*28] = image[j,:,:,:].reshape(28,28)

      plt.gray()
      plt.imshow(tmp.reshape([28*per_row,28*per_row]))
      plt.savefig("./images/" + str(i) + ".png")
      plt.gcf().clear()

    d_total_loss = [sum(x) for x in zip(d1_loss, d2_loss)]
    plt.plot(range(len(g_loss)), g_loss, label="Generator")
    plt.plot(range(len(d1_loss)), d1_loss, label="Discriminator 1")
    plt.plot(range(len(d2_loss)), d2_loss, label="Discriminator 2")
    plt.plot(range(len(d_total_loss)), d_total_loss, label="Discriminator Total")
    plt.xlabel("Iterations")
    plt.ylabel("Loss")
    plt.title("Generator and Discriminator Losses Over Time")
    plt.legend(loc="upper right")
    plt.savefig("./images/loss.png")
    plt.gcf().clear()
    dc_gan.save_params(path)

if __name__ == "__main__":
  main()