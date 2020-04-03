#include <stdio.h>
#include <cs50.h>
#include <string.h>

#define INVALID 0
#define AMEX 1
#define MASTERCARD 2
#define VISA 3


long prompt_card_number(void)
{
    long card_number;
    card_number = get_long("Number: \n");
    return card_number;
}


bool luhns_algorithm(long card_number)
{   
    char card_number_string[16];
    sprintf(card_number_string, "%lu", card_number);

    int checksum = 0;
    
    for (int i = 1; i <= strlen(card_number_string) ; i++)
    {
        int product_digit = 0;
        // If the digit's position is odd we need to multiply by 2 and sum the digits
        if (i % 2 == 0)
        {
            // Minus 48 to converte ASCII to int.
            product_digit = (card_number_string[strlen(card_number_string) - i] - 48) * 2;
            if (product_digit >= 10)
            {
                checksum += 1 + product_digit - 10;
            } 
            else
            {
                checksum += product_digit;
            }
        }
        else
        {
            checksum += card_number_string[strlen(card_number_string) - i] - 48;
        }
    }
    return (checksum % 10 == 0);
}

// Given a card number return the Provider ID
int get_provider(long card_number)
{
    int id_provider = 0;
    char card_number_string[16];
    sprintf(card_number_string, "%lu", card_number);

    int first_two_digits = (card_number_string[0] - 48) * 10 + (card_number_string[1] - 48);
    switch (first_two_digits)
    {
        case 34:
        case 37:
            id_provider = AMEX;
            break;

        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
            id_provider = MASTERCARD;
            break;

        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
            id_provider = VISA;
            break;

        default:
            id_provider = INVALID;
    }

    return id_provider;
}

// Checks if is a valid card number, first by the length and later with luhns algorithm
bool check_card_number(long card_number)
{
    bool valid_card = false;
    char card_number_string[16];
    sprintf(card_number_string, "%lu", card_number);

    if (strlen(card_number_string) >= 13)
    {
        if (luhns_algorithm(card_number))
        {
            valid_card = true;
        }
    }

    return valid_card;
}

// Prints the provider given a card number
void print_provider(long card_number)
{
    if (check_card_number(card_number))
    {
        switch (get_provider(card_number))
        {
            case AMEX:
              printf("AMEX\n");
              break;
            case MASTERCARD:
                printf("MASTERCARD\n");
                break;
            case VISA:
                printf("VISA\n");
                break;
            case INVALID:
                printf("INVALID\n");
                break;
        }
    }
    else
    {
        printf("INVALID\n");
    }
}


int main(void)
{
    long card_number = 0;
    card_number = prompt_card_number();
    print_provider(card_number);
}
