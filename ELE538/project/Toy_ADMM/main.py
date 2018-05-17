import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

from model import Toy_ADMM

def main():
  train()

def train(num_epochs=100, batch_size=100):
  path = "./checkpoints/model.ckpt"
  toy = Toy_ADMM()
  toy.set_params(path)

  f_loss = []
  g_loss = []
  total_loss = []

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

    diff, l2, prod, lamb = toy.get_variables()
    print("Diff: " + str(diff))
    print("l2: " + str(l2))
    print("prod: " + str(prod))
    print("lambda: " + str(lamb))

    toy.update_theta2()
    toy.update_lambda()

    diff, l2, prod, lamb = toy.get_variables()
    print("Diff: " + str(diff))
    print("l2: " + str(l2))
    print("prod: " + str(prod))
    print("lambda: " + str(lamb))

    print("\n" + "-"*20 + "\n")

    loss_f, loss_g, loss_total = toy.get_losses(x, y)

    print("Total Loss: " + str(loss_total))
    total_loss.append(loss_total)
    print("f Loss: " + str(loss_f))
    f_loss.append(loss_f)
    print("g Loss: " + str(loss_g))
    g_loss.append(loss_g)

    plt.plot(range(len(total_loss)), total_loss, label="Total")
    plt.plot(range(len(f_loss)), f_loss, label="f")
    plt.plot(range(len(g_loss)), g_loss, label="g")
    plt.xlabel("Iterations")
    plt.ylabel("Loss")
    plt.title("Losses Over Time")
    plt.legend(loc="upper right")
    plt.savefig("./images/loss.png")
    plt.gcf().clear()
    toy.save_params(path)

if __name__ == "__main__":
  main()