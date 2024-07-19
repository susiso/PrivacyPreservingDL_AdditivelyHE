import cv2
import tensorflow as tf
from tensorflow.keras import layers, models
import numpy as np
from keras.datasets import mnist
from keras.utils import to_categorical
import matplotlib.pyplot as plt
import seaborn as sns
import random

tf.random.set_seed(0)
random.seed(0)

def load_mnist():
	(x_train, y_train), (x_test, y_test) = mnist.load_data()

	# 論文に合うように20x20に削減
	x_train = np.array([cv2.resize(img, (20, 20)) for img in x_train])
	x_test = np.array([cv2.resize(img, (20, 20)) for img in x_test])

	x_train = x_train / 255
	x_test = x_test / 255
	
	y_train = to_categorical(y_train)
	y_test = to_categorical(y_test)
	
	return x_train, y_train, x_test, y_test

def general_neural_networks(x_train, y_train, x_test, y_test):
	# parameter
	batch_size = 200
	learning_rate = 1e-4
	epochs = 10

	# モデルの作成
	model = tf.keras.models.Sequential([
		tf.keras.layers.Flatten(input_shape=(20, 20)),
		tf.keras.layers.Dense(25, activation=tf.nn.relu, use_bias=True),
		tf.keras.layers.Dense(10, activation=tf.nn.softmax, use_bias=True)
	])
 
	# モデルのコンパイル
	model.compile(optimizer='sgd',
				loss='categorical_crossentropy',
				metrics=['accuracy'])

	# モデルの概要を表示
	model.summary()
 
	# モデルの訓練
	r = model.fit(x_train, y_train, validation_data=(x_test, y_test), epochs=epochs)

	# 勾配を求める x_test[3]が0の画像
	input_data = x_test[3].reshape(-1, 20, 20)
	target_data = y_test[3].reshape(-1, 10)
 
	with tf.GradientTape() as tape:
		# 順伝播
		predictions = model(input_data)
		# 損失の計算
		loss = tf.keras.losses.categorical_crossentropy(target_data, predictions)

	# 勾配の計算
	gradients = tape.gradient(loss, model.layers[1].trainable_variables)

	grad_part = np.zeros(400)
	for i in range(400):
		grad_part[i] = gradients[0][i, 0]

	return input_data, grad_part


def general_neural_networks_with_regularization(x_train, y_train, x_test, y_test):
	# parameter
	batch_size = 200
	learning_rate = 1e-4
	epochs = 10
	lam = 0.001

	# モデルの作成
	model = tf.keras.models.Sequential([
		tf.keras.layers.Flatten(input_shape=(20, 20)),
		tf.keras.layers.Dense(25, activation=tf.nn.relu, use_bias=True, kernel_regularizer=tf.keras.regularizers.l2(lam)),
		tf.keras.layers.Dense(10, activation=tf.nn.softmax, use_bias=True, kernel_regularizer=tf.keras.regularizers.l2(lam))
	])
 
	# モデルのコンパイル
	model.compile(optimizer='sgd',
				loss='categorical_crossentropy',
				metrics=['accuracy'])

	# モデルの概要を表示
	model.summary()
 
	# モデルの訓練
	r = model.fit(x_train, y_train, validation_data=(x_test, y_test), epochs=epochs)

	W = model.layers[1].get_weights()[0]

	# 勾配を求める x_test[3]が0の画像
	input_data = x_test[3].reshape(-1, 20, 20)
	target_data = y_test[3].reshape(-1, 10)
 
	with tf.GradientTape() as tape:
		# 順伝播
		predictions = model(input_data)
		# 損失の計算
		loss = tf.keras.losses.categorical_crossentropy(target_data, predictions)

	# 勾配の計算
	gradients = tape.gradient(loss, model.layers[1].trainable_variables)
 
	grad_part = np.zeros(400)
	for i in range(400):
		grad_part[i] = (gradients[0][i, 0] + lam * W[i, 0])/ gradients[1][0]
	print(grad_part)

	return grad_part

def laplace_noise_add(grad_part):
	loc = 0
	scale = 0.000005
	grad_part_noise = grad_part + np.random.laplace(loc, scale, size=(400))
	return grad_part_noise

def main():
	x_train, y_train, x_test, y_test = load_mnist()
	input_data, grad_part_b = general_neural_networks(x_train, y_train, x_test, y_test)
	grad_part_c = general_neural_networks_with_regularization(x_train, y_train, x_test, y_test)
	grad_part_d = laplace_noise_add(grad_part_b)
 
	input_data = input_data.reshape(20, 20)
	grad_part_b = grad_part_b.reshape(20, 20)
	grad_part_c = grad_part_c.reshape(20, 20)
	grad_part_d = grad_part_d.reshape(20, 20)

	# (a)
	plt.figure()
	sns.heatmap(input_data, square=True, xticklabels=False, yticklabels=False, cmap="jet", vmin=-0.05, vmax=1.05)
	plt.savefig("./result/a.png")
	plt.close()

	# (b)
	plt.figure()
	sns.heatmap(grad_part_b, square=True, xticklabels=False, yticklabels=False, cmap="jet", vmin=-0.1*1e-5, vmax=4.7*1e-5)
	plt.savefig("./result/b.png")
	plt.close()

	# (c)
	plt.figure()
	sns.heatmap(grad_part_c, square=True, xticklabels=False, yticklabels=False, cmap="jet")
	plt.savefig("./result/c.png")
	plt.close()
 
	# (d)
	plt.figure()
	sns.heatmap(grad_part_d, square=True, xticklabels=False, yticklabels=False, cmap="jet")
	plt.savefig("./result/d.png")
	plt.close()
 
    
if __name__ == "__main__":
    main()