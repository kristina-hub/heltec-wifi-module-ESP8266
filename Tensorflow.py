# Import libraries
import tensorflow as tf
from tensorflow.keras import preprocessing
import matplotlib.pyplot as plt
import numpy as np

# Model parameters
MAX_FEATURES = 10000
MAX_LEN = 500  # Each movie review is paddded/truncated to 50 tokens
#WORD_EMB_DIM = 256  # Dimension of each word embedding
WORD_EMB_DIM = 32  # Dimension of each word embedding

# Training parameters
NUM_EPOCHS = 10
BATCH_SIZE = 64

# Import data
imdb = tf.keras.datasets.imdb
(X_train, y_train), (X_test, y_test) = imdb.load_data(num_words=MAX_FEATURES)

# Inspect training instance
print(X_train[0])
print(y_train[0])

print(max([max(sequence) for sequence in X_train]))
print(max([max(sequence) for sequence in X_test]))

# word_index is a dictionary mapping words to an integer index
word_index = imdb.get_word_index()

# We reverse it, mapping integer indices to words
reverse_word_index = dict([(value, key) for (key, value) in word_index.items()])

# We decode the review; note that our indices were offset by 3
# because 0, 1 and 2 are reserved indices for "padding", "start of sequence", and "unknown".
decoded_review = ' '.join([reverse_word_index.get(i - 3, '?') for i in X_train[0]])

print(word_index)
print(decoded_review)

# Pad integer sequences to length of 50
X_train = preprocessing.sequence.pad_sequences(X_train, maxlen=MAX_LEN, truncating="post")
X_test = preprocessing.sequence.pad_sequences(X_test, maxlen=MAX_LEN, truncating="post")

print(X_train[0])

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Flatten, Dense, Embedding, Dropout

model = Sequential()
model.add(Embedding(MAX_FEATURES, WORD_EMB_DIM, input_length=MAX_LEN)) # Shape: (num_samples, MAX_LEN, WORD_EMB_DIM)
model.add(Flatten()) # Shape: (num_samples, MAX_LEN * WORD_EMB_DIM)
model.add(Dropout(0.3))  # A dropout layer randomly ignores a proportion of the nodes
#model.add(Dropout(0.3))
#ignore 30% of nodes in backpropogation to prevent overfitting
model.add(Dense(100, activation='relu'))
model.add(Dense(1, activation='sigmoid'))
optimizer = tf.keras.optimizers.Adam(learning_rate=0.0001, beta_1=0.9, beta_2=0.999, epsilon=1e-07, amsgrad=False)
model.compile(optimizer=optimizer, loss='binary_crossentropy', metrics=['acc'])
#model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['acc'])
#model.compile(optimizer='sgd', loss='binary_crossentropy', metrics=['acc'])
print(model.summary())

history = model.fit(X_train, y_train,
                    epochs=NUM_EPOCHS,
                    batch_size=BATCH_SIZE,
                    validation_split=0.2)


results = model.evaluate(X_test, y_test)
print(results)
