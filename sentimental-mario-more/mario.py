from cs50 import get_int

# gets height as user input
while True:
    height = get_int("Height: ")
    if height in range(1, 9):
        break

# prints spaces and blocks
for i in range(1, height + 1):
    print(" " * (height - i), end="")
    print("#" * i, end="")
    print("  ", end="")
    print("#" * i)