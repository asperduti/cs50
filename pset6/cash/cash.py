from cs50 import get_float


valid_coins = (25, 10, 5, 1)


def main():
    # Prompt to the user
    change_owed = prompt_user("Change owed: ")
    # Convert the value to cents
    change_owed *= 100
    total_coins = 0

    # Calculate the coins
    for coin in valid_coins:
        coins, change_owed = divmod(change_owed, coin)
        total_coins += coins

    print(f"{total_coins:.0f}")


def prompt_user(msg):
    # Prompt until get a valid value
    while True:
        user_input = get_float(msg)
        if user_input >= 0:
            return user_input


if __name__ == "__main__":
    main()