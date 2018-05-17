import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

from model import Toy

"""
Final Loss = ~1400-1600
"""

def main():
  train()

def train(num_epochs=100, batch_size=100):
  path = "./checkpoints/model.ckpt"
  toy = Toy()
  toy.set_params(path)

  total_loss = []
  mse_loss = []
  regularizer_loss = []

  train_x = np.random.normal(0, 1, size=(10000,25,25)).astype(np.float32)

  # random linear model
  A = np.random.uniform(-5,5, size=(5,25,25)).astype(np.float32)
  B = np.random.uniform(-5,5, size=(5)).astype(np.float32)

  train_y = np.array([[np.sum(x*a) + b for a, b in zip(A,B)] for x in train_x])

  for i in range(num_epochs):
    print("=== In Epoch " + str(i) + " ===")

    # loop through whole training data in random order
    index = np.arange(len(train_x))
    np.random.shuffle(index)
    train_x = train_x[index]
    train_y = train_y[index]

    for start, end in zip(range(0, len(train_x), batch_size), range(batch_size, len(train_x), batch_size)):
      x = train_x[start:end].astype(np.float32)
      y = train_y[start:end].astype(np.float32)

      toy.train(x, y)

    loss_total, loss_mse, loss_regularizer = toy.get_losses(x, y)

    print("Total Loss: " + str(loss_total))
    total_loss.append(loss_total)
    print("MSE Loss: " + str(loss_mse))
    mse_loss.append(loss_mse)
    print("Regularizer Loss: " + str(loss_regularizer))
    regularizer_loss.append(loss_regularizer)

    plt.plot(range(len(total_loss)), total_loss, label="Total")
    plt.plot(range(len(mse_loss)), mse_loss, label="MSE")
    plt.plot(range(len(regularizer_loss)), regularizer_loss, label="Regularizer")
    plt.xlabel("Iterations")
    plt.ylabel("Loss")
    plt.title("Losses Over Time")
    plt.legend(loc="upper right")
    plt.savefig("./images/loss.png")
    plt.gcf().clear()
    toy.save_params(path)

if __name__ == "__main__":
  main()