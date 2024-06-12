import os

os.environ["TF_ENABLE_ONEDNN_OPTS"] = "0"
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"

import subprocess

import numpy as np
import pandas as pd
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import OneHotEncoder

df = pd.read_csv("dataset/dataset.csv")
X = df.drop("target", axis=1).to_numpy()

one_hot_encoder = OneHotEncoder()
y = one_hot_encoder.fit_transform(df["target"].to_numpy().reshape(-1, 1)).toarray()
labels = one_hot_encoder.categories_[0]


X_train, X_test, y_train, y_test = train_test_split(
    X,
    y,
    test_size=0.2,
    random_state=42,
    stratify=y,
)

# Train model
model = tf.keras.Sequential(
    [
        tf.keras.Input(shape=(X.shape[1],)),
        tf.keras.layers.Dense(64, activation="relu"),
        tf.keras.layers.Dense(64, activation="relu"),
        tf.keras.layers.Dense(64, activation="relu"),
        tf.keras.layers.Dense(y.shape[1], activation="softmax"),
    ]
)

model.compile(optimizer="adam", loss="categorical_crossentropy", metrics=["accuracy"])
model.fit(X_train, y_train, epochs=750, batch_size=64, verbose=1)
loss, accuracy = model.evaluate(X_test, y_test, verbose=0)
print(f"Test Accuracy: {accuracy}")

y_pred = model.predict(X_test)
predicted_classes = np.argmax(y_pred, axis=1)
true_classes = np.argmax(y_test, axis=1)

for i in range(10):
    print(f"True: {true_classes[i]}, Predicted: {predicted_classes[i]}")
model.export("model")

# Convert the model
converter = tf.lite.TFLiteConverter.from_saved_model("model")
tflite_model = converter.convert()

# Save the model
with open("model.tflite", "wb") as f:
    f.write(tflite_model)
