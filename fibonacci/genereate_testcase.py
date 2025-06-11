import math
import os
import random

os.makedirs("output", exist_ok=True)
os.makedirs("input", exist_ok=True)

fibonacci = [0, 1]
for j in range(2, 101):
    fibonacci.append(fibonacci[j - 1] + fibonacci[j - 2])

for i in range(1, 21):
    n = random.randint(3, 40)
    content = f"n 0 {n}\nresult 4 {fibonacci[n]}\n"
    fname = f"output/output{i:02d}.txt"
    with open(fname, "w") as f:
        f.write(content)

    content = f"n 0 {n}\nresult 4 0\n"
    fname = f"input/input{i:02d}.txt"
    with open(fname, "w") as f:
        f.write(content)