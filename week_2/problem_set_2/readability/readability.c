#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// ? function prototypes
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int formula(int letter_count, int word_count, int sentence_count);

int main(void)
{
    string text = get_string("Text: ");
    int letter_count = count_letters(text);
    int word_count = count_words(text);
    int sentence_count = count_sentences(text);
    int index = formula(letter_count, word_count, sentence_count);

// ? return grade
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 1 && index <= 15)
    {
        printf("Grade %i\n", index);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

// ? return the count of letters (or numbers) for the entire string
int count_letters(string text)
{
    int letter_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalnum(text[i]))
        {
            letter_count = letter_count + 1;
        }
    }
    return letter_count;
}

// ? return number of words for the entire string
int count_words(string text)
{
    int word_count = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isspace(text[i]))
        {
            word_count = word_count + 1;
        }
    }
    return word_count;
}

// ? return number of sentences for the entire string
int count_sentences(string text)
{
    int sentence_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence_count = sentence_count + 1;
        }
    }
    return sentence_count;
}

// ? apply the Coleman-Liau formula
int formula(int letter_count, int word_count, int sentence_count)
{
    float L = (letter_count * 100.0) / word_count;
    L = roundf(L * 100) / 100;

    float S = (sentence_count * 100.0) / word_count;
    S = roundf(S * 100) / 100;

    return round(0.0588 * L - 0.296 * S - 15.8);
}