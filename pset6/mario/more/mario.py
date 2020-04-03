from cs50 import get_int


def main():
    # Ask to user
    height = prompt_height()
    # Draw the pyramid
    for step in range(1, height + 1):
        draw_step(height=height, width=step)


def prompt_height():
    # Prompt height until get a valid value
    while True:
        height = get_int("Height: ")
        if height >= 1 and height <= 8:
            return height


def draw_step(height, width):
    # Draw the step
    print(" " * (height - width), end="")
    print("#" * width, end="")
    print("  ", end="")
    print("#" * width)


if __name__ == "__main__":
    main()
