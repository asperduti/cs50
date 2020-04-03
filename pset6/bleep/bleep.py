from cs50 import get_string
from sys import argv


def main():
    # Check command line arguments
    if len(argv) != 2:
        print(f"Usage python {argv[0]} file.txt")
        exit(1)
    # Open the file and load the words
    file = argv[1]
    banned_words = set()
    with open(file, "r") as f:
        for line in f:
            banned_words.add(line.rstrip("\n"))

    # Prompt to user for the text to bleep
    text = get_string("What message would you like to censor?\n")

    # Extract the words in the user input
    words = text.split()
    censored_text = ""
    # Check if the words is in the list of banned words
    for word in words:
        # If it's a banned words replace with *
        if word.lower() in banned_words:
            censored_text += "*" * len(word) + " "
        else:
            censored_text += word + " "

    print(censored_text)


if __name__ == "__main__":
    main()
