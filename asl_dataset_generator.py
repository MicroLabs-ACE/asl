import random

import pandas as pd

random.seed(42)

flex_sensor_ranges = {
    "A": {
        "thumb": (0, 15),
        "index": (70, 100),
        "middle": (70, 100),
        "ring": (70, 100),
        "pinky": (70, 100),
    },
    "B": {
        "thumb": (50, 75),
        "index": (0, 15),
        "middle": (0, 15),
        "ring": (0, 15),
        "pinky": (0, 15),
    },
    "C": {
        "thumb": (20, 45),
        "index": (50, 75),
        "middle": (50, 75),
        "ring": (50, 75),
        "pinky": (50, 75),
    },
    "D": {
        "thumb": (30, 50),
        "index": (0, 15),
        "middle": (55, 80),
        "ring": (55, 80),
        "pinky": (55, 80),
    },
    "E": {
        "thumb": (80, 100),
        "index": (75, 100),
        "middle": (75, 100),
        "ring": (75, 100),
        "pinky": (75, 100),
    },
    "F": {
        "thumb": (40, 75),
        "index": (55, 80),
        "middle": (75, 95),
        "ring": (70, 100),
        "pinky": (70, 100),
    },
    "G": {
        "thumb": (60, 80),
        "index": (60, 90),
        "middle": (70, 100),
        "ring": (70, 100),
        "pinky": (70, 100),
    },
    "H": {
        "thumb": (75, 95),
        "index": (60, 90),
        "middle": (60, 90),
        "ring": (80, 100),
        "pinky": (80, 100),
    },
    "I": {
        "thumb": (80, 100),
        "index": (70, 100),
        "middle": (70, 100),
        "ring": (70, 100),
        "pinky": (0, 20),
    },
    "J": {
        "thumb": (75, 95),
        "index": (90, 100),
        "middle": (90, 100),
        "ring": (90, 100),
        "pinky": (15, 45),
    },
    "K": {
        "thumb": (30, 60),
        "index": (10, 40),
        "middle": (10, 40),
        "ring": (70, 100),
        "pinky": (0, 20),
    },
    "L": {
        "thumb": (40, 65),
        "index": (0, 15),
        "middle": (70, 100),
        "ring": (70, 100),
        "pinky": (70, 100),
    },
    "M": {
        "thumb": (90, 100),
        "index": (85, 100),
        "middle": (85, 100),
        "ring": (85, 100),
        "pinky": (90, 100),
    },
    "N": {
        "thumb": (85, 100),
        "index": (85, 100),
        "middle": (85, 100),
        "ring": (90, 100),
        "pinky": (90, 100),
    },
    "O": {
        "thumb": (30, 50),
        "index": (60, 80),
        "middle": (60, 80),
        "ring": (60, 80),
        "pinky": (65, 85),
    },
    "P": {
        "thumb": (80, 95),
        "index": (70, 90),
        "middle": (80, 90),
        "ring": (90, 100),
        "pinky": (90, 100),
    },
    "Q": {
        "thumb": (60, 80),
        "index": (70, 80),
        "middle": (80, 100),
        "ring": (80, 100),
        "pinky": (80, 100),
    },
    "R": {
        "thumb": (70, 90),
        "index": (30, 50),
        "middle": (80, 100),
        "ring": (80, 100),
        "pinky": (80, 100),
    },
    "S": {
        "thumb": (90, 100),
        "index": (90, 100),
        "middle": (90, 100),
        "ring": (90, 100),
        "pinky": (90, 100),
    },
    "T": {
        "thumb": (75, 85),
        "index": (85, 95),
        "middle": (85, 100),
        "ring": (85, 100),
        "pinky": (85, 100),
    },
    "U": {
        "thumb": (75, 95),
        "index": (0, 10),
        "middle": (0, 10),
        "ring": (85, 100),
        "pinky": (85, 100),
    },
    "V": {
        "thumb": (75, 95),
        "index": (5, 15),
        "middle": (5, 15),
        "ring": (85, 100),
        "pinky": (85, 100),
    },
    "W": {
        "thumb": (75, 95),
        "index": (5, 15),
        "middle": (5, 15),
        "ring": (5, 15),
        "pinky": (85, 100),
    },
    "X": {
        "thumb": (85, 100),
        "index": (75, 100),
        "middle": (80, 100),
        "ring": (80, 100),
        "pinky": (80, 100),
    },
    "Y": {
        "thumb": (75, 90),
        "index": (85, 100),
        "middle": (85, 100),
        "ring": (85, 100),
        "pinky": (0, 15),
    },
    "Z": {
        "thumb": (75, 100),
        "index": (15, 50),
        "middle": (75, 100),
        "ring": (75, 100),
        "pinky": (75, 100),
    },
}

fingers = ["thumb", "index", "middle", "ring", "pinky"]
dataset_dict = {finger: list() for finger in [*fingers, "target"]}

for letter, ranges in flex_sensor_ranges.items():
    for _ in range(250):
        for finger, rnge in ranges.items():
            dataset_dict[finger].append(random.randint(*rnge))
        dataset_dict["target"].append(letter)


pd.DataFrame(dataset_dict).to_csv("dataset/dataset.csv", index=False)
