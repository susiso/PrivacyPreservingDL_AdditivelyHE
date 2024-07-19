import tensorflow as tf
from tensorflow.keras.datasets import mnist
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.optimizers import Adam
import random
import time

tf.random.set_seed(0)
random.seed(0)

# MNISTデータセットの読み込み
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()

# データの前処理
train_images = train_images.reshape(60000, 784).astype('float32') / 255
test_images = test_images.reshape(10000, 784).astype('float32') / 255

# ne-hotエンコーディング
train_labels = tf.keras.utils.to_categorical(train_labels, 10)
test_labels = tf.keras.utils.to_categorical(test_labels, 10)

# モデルの構築
model = Sequential([
    Dense(128, activation='relu', input_shape=(784,), kernel_initializer=tf.keras.initializers.RandomNormal(mean=0.0, stddev=0.1)),
    Dense(64, activation='relu', kernel_initializer=tf.keras.initializers.RandomNormal(mean=0.0, stddev=0.1)),
    Dense(10, activation='softmax', kernel_initializer=tf.keras.initializers.RandomNormal(mean=0.0, stddev=0.1))
])

# モデルのコンパイル
model.compile(optimizer=Adam(learning_rate=1e-4),
              loss='categorical_crossentropy',
              metrics=['accuracy'])

# モデルの訓練
start = time.time()
model.fit(train_images, train_labels, batch_size=50, epochs=10, validation_data=(test_images, test_labels))
end = time.time()  
time_diff = end - start  # 処理完了後の時刻から処理開始前の時刻を減算する

# モデルの評価
test_loss, test_acc = model.evaluate(test_images, test_labels)
print(f'Test accuracy: {test_acc}')
print("time elapsed : ", {time_diff})