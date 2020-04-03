#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

const char DICTIONARY[] = "abc";
const int PASSWORD_LENGTH = 5;

bool crack(int length_password, string password, string hash)
{
    bool password_founded = false;
    if (length_password <= 0)
    {
        return false;
    }
    for (int i = 0, dict_lenght = strlen(DICTIONARY); i < dict_lenght; i++)
    {
        if (length_password == 1)
        {
            if (strcmp(password, hash) == 0)
            {
                password_founded = true;
                break;
            }   
        }
        password_founded = crack(length_password - 1, password, hash);
        if (!password_founded) 
        {
            password[length_password - 1] = DICTIONARY[i];
        }
    }
    return password_founded;
}

int main(int argc, string argv[])
{
    // Check correct ammount of arguments
    if (argc != 2)
    {
        printf("Usage: %s hash\n", argv[0]);
        return 1;
    }
    
    string hash = argv[1];
    // The array has an extra elemente for the null char
    char password[PASSWORD_LENGTH];
    if (crack(PASSWORD_LENGTH, password, hash))
    {
        printf("%s\n", password);
    }
}
