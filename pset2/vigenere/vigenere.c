#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int shift(char c)
{
    char shifter = 0;
    
    if (c >= 'a' && c <= 'z')
    {
        shifter = 'a';
    }
    
    if (c >= 'A' && c <= 'Z')
    {
        shifter = 'A';
    }
    
    return c - shifter;
}

string vigenere(string keyword, string plain_text)
{
    int char_keyword = 0;
    // Iterate over every char
    for (int i = 0, length = strlen(plain_text); i < length; i++)
    {
        // Test if is lower or upper char to fit
        if islower(plain_text[i])
        {
            plain_text[i] = 'a' + (plain_text[i] - 'a' + shift(keyword[char_keyword])) % 26;
            char_keyword = (char_keyword + 1) % strlen(keyword);
        }
        
        if isupper(plain_text[i])
        {
            plain_text[i] = 'A' + (plain_text[i] - 'A' + shift(keyword[char_keyword])) % 26;
            char_keyword = (char_keyword + 1) % strlen(keyword);
        }
         
    }
    
    return plain_text;
}

int main(int argc, string argv[])
{
    
    // Check amount of arguments
    if (argc != 2)
    {
        printf("Usage: %s keyword\n", argv[0]);
        return 1;
    }
    
    // Check that the keyword is full character
    for (int i = 0, length = strlen(argv[1]); i < length; i++)
    {
        char character = argv[1][i];
        // I can use the shift() previusly declared
        if (shift(character) < 0 || shift(character) > 25)
        {
            printf("Usage: %s keyword\n", argv[0]);
            return 1;
        }
    }
    
    // Ask for plain text
    string plain_text = get_string("plaintext: ");
    
    vigenere(argv[1], plain_text);
    
    printf("ciphertext: %s\n", plain_text);
    // Exit without error
    return 0;
}
