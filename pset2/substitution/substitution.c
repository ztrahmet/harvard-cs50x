#include <cs50.h>  // string, get_string()
#include <ctype.h> // isalpha(), isupper(), islower(), tolower(), toupper()
#include <stdio.h> // printf

string encrypt(const string, string);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    // Assign the key to a constant
    const string KEY = argv[1];

    int len = 0;

    for (; KEY[len] != '\0'; len++) // Iterate over the key characters
    {
        if (!isalpha(KEY[len])) // Check for not alphabetic character
        {
            printf("Key must contain alphabetic characters.\n");
            return 1;
        }
        for (int i = 0; i < len; i++) // Check double character
        {
            if (tolower(KEY[i]) == tolower(KEY[len])) // Case insensitively
            {
                printf("Key must contain unique characters.\n");
                return 1;
            }
        }
    }

    if (len != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Get the text from the user
    string plaintext = get_string("plaintext:  ");

    // Encrypt it
    string ciphertext = encrypt(KEY, plaintext);

    // Print out the cipher text
    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

string encrypt(const string KEY, string text)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (islower(text[i]))
        {
            text[i] = tolower(KEY[text[i] - 'a']);
        }
        else if (isupper(text[i]))
        {
            text[i] = toupper(KEY[text[i] - 'A']);
        }
    }
    return text;
}
