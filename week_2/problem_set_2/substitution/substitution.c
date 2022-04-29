#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool is_valid_key(int argc, string key);
string encipher(string cipher_code, string plaintext);

// ? Get key
int main(int argc, string argv[])
{
    if (is_valid_key(argc, argv[1]))
    {
        string encipher(string cipher_code, string plaintext);
// ? Get plaintext
        string plaintext = get_string("plaintext: ");
        string ciphertext = encipher(argv[1], plaintext);
// ? Print ciphertext
        printf("ciphertext: %s\n", ciphertext);
        return 0;
    }
    return 1;
}

// ? Validade tke key
bool is_valid_key(int argc, string key)
{
// ? Check number of arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return false;
    }

// ? Check key length
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return false;
    }

// ? Check for non-alphabetic characters
    for (int i = 0; i < strlen(key); i++)
    {
        if (isalpha(key[i]) == false)
        {
            printf("Usage: ./substitution key\n");
            return false;
        }
    }

// ? Check for repeated characters (case-insensitive)
    for (int i = 0; i < strlen(key); i++)
    {
        int letter_count = 0;
        for (int j = 0; j < strlen(key); j++)
        {
            if (key[i] == key[j])
            {
                letter_count = letter_count + 1;
                if (letter_count > 1)
                {
                    printf("Key must not contain repeated characters.\n");
                    return false;
                }
            }
        }
    }
    return true;
}

// ? Encipher
string encipher(string cipher_code, string plaintext)
{
// ? For each alphabetic character, determine what letter it maps to
    string ciphertext = plaintext;
    for (int i = 0; i < strlen(plaintext); i++)
    {
// ? Leave non-alphabetic characters as-is
        if (islower(plaintext[i]))
        {
            int char_index = (int) plaintext[i] - 97;
// ? Preserve case
            ciphertext[i] = tolower(cipher_code[char_index]);
        }
// ? Leave non-alphabetic characters as-is
        else if (isupper(plaintext[i]))
        {
            int char_index = (int) plaintext[i] - 65;
// ? Preserve case
            ciphertext[i] = toupper(cipher_code[char_index]);
        }
    }
    return ciphertext;
}