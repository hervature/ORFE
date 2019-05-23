import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

from model import DCGAN

def main():
  train()

def train(num_epochs=1000, batch_size=128):

  path = "./checkpoints/model.ckpt"
  mnist = tf.contrib.learn.datasets.load_dataset("mnist")
  train_data = mnist.train.images.reshape(-1,28,28,1)

  dc_gan = DCGAN()
  dc_gan.set_params(path)
  g_loss = []
  d_loss = []

  for i in range(num_epochs):
    print("=== In Epoch " + str(i) + " ===")

    # loop through whole training data in random order
    index = np.arange(len(train_data))
    np.random.shuffle(index)
    train_data = train_data[index]

    for start, end in zip(range(0, len(train_data), batch_size), range(batch_size, len(train_data), batch_size)):
      Z = np.random.uniform(-1,1, size=(batch_size, dc_gan.dim_z)).astype(np.float32)
      image = train_data[start:end].astype(np.float32)

      if i % 2 == 0:
        # first train generator
        dc_gan.train_generator(Z)
      else:
        # second train discriminator
        dc_gan.train_discriminator(Z, image)

    generator_loss, discriminator_loss = dc_gan.get_losses(Z, image)
    if i % 2 == 1:
      print("Generator Loss: " + str(generator_loss))
      g_loss.append(generator_loss)
    else:
      print("Discriminator Loss: " + str(discriminator_loss))
      d_loss.append(discriminator_loss)

    if i % 2 == 0:
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

    plt.plot(range(len(g_loss)), g_loss, label="Generator")
    plt.plot(range(len(d_loss)), d_loss, label="Discriminator")
    plt.xlabel("Iterations")
    plt.ylabel("Loss")
    plt.title("Generator and Discriminator Losses Over Time")
    plt.legend(loc="upper right")
    plt.savefig("./images/loss.png")
    plt.gcf().clear()
    dc_gan.save_params(path)

if __name__ == "__main__":
  main()