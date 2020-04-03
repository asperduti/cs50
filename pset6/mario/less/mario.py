from cs50 import get_int

# Ask user input until get a valid input
while True:
    height = get_int("Height: ")
    if height >= 1 and height <= 8:
        break

for i in range(1, height + 1):
    # Print the empty scape to align the hash
    for j in range(height - i):
        print(" ", end="")
    # Print the amount of hash acording to the level
    for j in range(i):
        print("#", end="")

    print()