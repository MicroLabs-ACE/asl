import os
import random

random.seed(42)
os.environ["TF_ENABLE_ONEDNN_OPTS"] = "0"
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"

import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split


# Generate data
top_left_quadrant_range = ((0, 99), (0, 99))
top_right_quadrant_range = ((100, 199), (0, 100))
bottom_left_quadrant_range = ((0, 99), (100, 199))
bottom_right_quadrant_range = ((100, 199), (100, 199))

X = []
y = []

for _ in range(4000):
    ranges = (
        top_left_quadrant_range,
        top_right_quadrant_range,
        bottom_left_quadrant_range,
        bottom_right_quadrant_range,
    )
    choice_ranges = random.choice(ranges)
    horizontal_range, vertical_range = choice_ranges
    horizontal = random.randint(*horizontal_range)
    vertical = random.randint(*vertical_range)
    target = [0 for _ in range(4)]
    target_index = ranges.index(choice_ranges)
    target[target_index] = 1

    X.append([horizontal, vertical])
    y.append(target)

X = np.array(X)
y = np.array(y)

X_train, X_test, y_train, y_test = train_test_split(
    X,
    y,
    test_size=0.2,
    random_state=42,
)

# Train model
model = tf.keras.Sequential(
    [
        tf.keras.Input(shape=(2,)),
        tf.keras.layers.Dense(8, activation="relu"),
        tf.keras.layers.Dense(8, activation="relu"),
        tf.keras.layers.Dense(8, activation="relu"),
        tf.keras.layers.Dense(4, activation="softmax"),
    ]
)

model.compile(optimizer="adam", loss="categorical_crossentropy", metrics=["accuracy"])
model.fit(X_train, y_train, epochs=100, batch_size=16, verbose=1)
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
