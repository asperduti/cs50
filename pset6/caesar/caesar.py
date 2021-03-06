from sys import argv, exit
from cs50 import get_string


def main():
    # Check amount of arguments
    if len(argv) != 2:
        print("usage:")
        print(f"python {argv[0]} k")
        exit(1)
    # Get the key and convert to int
    key = int(argv[1])

    # Prompt user
    plain_text = get_string("plaintext: ")
    # Encrypt and print the cipher text
    print(f"ciphertext: {encrypt(key, plain_text)}")


def encrypt(key, text):
    cipher_text = ""
    # Iterate over every character to check if it's alphabetic
    for c in text:
        if c.isalpha():
            # Set value to translate to the space 0-26 acording if it's lower or upper
            if c.isupper():
                mapper = ord('A')
            elif c.islower():
                mapper = ord('a')
            # Translate to the space 0-26
            c = ord(c) - mapper
            # Cipher
            c = (c + key) % 26
            # Translate to the char space and convert to char
            c = chr(c + mapper)
        cipher_text += c

    return cipher_text


if __name__ == "__main__":
    main()