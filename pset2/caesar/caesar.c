#include <cs50.h>
#include <stdio.h>
#include <string.h>

string encrypt(int key, string plaintext)
{

    for (int i = 0, length = strlen(plaintext); i < length; i++)
    {

        // Check that the char is a lower letter
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            // Cipher in the space of lowers letter
            plaintext[i] = 'a' + (plaintext[i] - 'a' + key) % ('a' - 'z' - 1);
        }

        // Check that the char is a upper letter
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            // Ciphe in the space of upper letter
            plaintext[i] = 'A' + (plaintext[i] + key - 'A') % ('A' - 'Z' - 1);
        }
    }

    return plaintext;
}

int main(int argc, string argv[])
{
    // Check if there is only one argument
    if (argc != 2)
    {
        printf("Usage ./%s key\n", argv[0]);
        return 1;
    }

    // Check that the key is a number
    for (int i = 0, length = strlen(argv[1]); i < length ; i++)
    {
        if (argv[1][i] < '0' || argv[1][i] > '9')
        {
            printf("Usage ./%s key\n", argv[0]);
            return 1;
        }
    }

    // Assing the key
    int key = atoi(argv[1]);

    // Ask for the text to ciphe
    string plain_text = get_string("plaintext:");

    // Print the ciphertext
    printf("ciphertext: %s\n", encrypt(key, plain_text));

    return 0;
}
