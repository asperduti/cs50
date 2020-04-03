from cs50 import get_int


def main():
    # Promt to the user the card number
    credit = prompt_credit()
    # Check if it's a valid number and print result
    if luhns(credit):
        if credit[0] == "4":
            print("VISA")
        elif credit[0:2] == "34" or credit[0:2] == "37":
            print("AMEX")
        elif credit[0:2] == "51" or credit[0:2] == "52" or credit[0:2] == "53" or credit[0:2] == "54" or credit[0:2] == "55":
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


def prompt_credit():
    # Promt the user until get a credit card number
    while True:
        credit = str(get_int("Number: "))
        return credit


def luhns(credit):
    # Check a valid length
    if len(credit) < 13 and len(credit) > 16:
        return False

    checksum = 0
    # Make the checksum
    for i in range(1, len(credit) + 1):
        if i % 2 == 0:
            product_digit = int(credit[len(credit) - i]) * 2
            if product_digit >= 10:
                checksum += 1 + product_digit - 10
            else:
                checksum += product_digit
        else:
            checksum += int(credit[len(credit) - i])

    return checksum % 10 == 0


if __name__ == "__main__":
    main()