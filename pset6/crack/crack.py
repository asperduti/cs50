import crypt
from sys import argv, exit

chars = " abcdefghijklmnopqrstvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

def main():
    # Check correct use
    if len(argv) != 2:
        print(f"Usage: python {argv[0]} password_hashed")
        exit(1)

    password_hashed = argv[1]

    password = crack(password_hashed)
    print(password)
    return


def crack(password_hashed):
    for fifth in chars:
        for fourth in chars:
            for tirth in chars:
                for second in chars:
                    for first in chars:
                        password = f"{first}{second}{tirth}{fourth}{fifth}".strip()
                        if crypt.crypt(password, password_hashed) == password_hashed:
                            print("Founded!")
                            return word


if __name__ == "__main__":
    main()