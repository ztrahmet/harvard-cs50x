from cs50 import get_int

# Prompt until valid height
while (True):
    height = get_int("Height: ")
    if 1 <= height <= 8:
        break

# Build pyramid
for i in range(1, height + 1):
    print(f"{' ' * (height - i)}{'#' * i}  {'#' * i}")
