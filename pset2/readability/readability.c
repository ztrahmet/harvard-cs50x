#include <cs50.h>   // string, get_string()
#include <ctype.h>  // isblank()
#include <math.h>   // round()
#include <stdio.h>  // printf()
#include <string.h> // strlen()

int get_letter_count(int, string);
int get_word_count(int, string);
int get_sentence_count(int, string);

int main(void)
{
    // Get text from the user
    string text = get_string("Text: ");
    int len = strlen(text);

    // Count letters, words and sentences
    int letter_count = get_letter_count(len, text), word_count = get_word_count(len, text),
        sentence_count = get_sentence_count(len, text), index;

    // Calculate the index
    float L = ((float) letter_count / word_count) * 100,
          S = ((float) sentence_count / word_count) * 100;

    index = round(0.0588 * L - 0.296 * S - 15.8);

    // View the values
    // printf("Letter Count:               %i\n", letter_count);
    // printf("Word Count:                 %i\n", word_count);
    // printf("Sentence Count:             %i\n", sentence_count);
    // printf("L(letters per 100 words):   %f\n", L);
    // printf("L(sentences per 100 words): %f\n", S);
    // printf("Index:                      %i\n", index);

    // Print the correct output
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

    return 0;
}

int get_letter_count(int len, string text)
{
    int count = 0;

    for (int i = 0; i < len; i++)
    {
        if (isalpha(text[i])) // Letter count is count of the alphaetic characters
        {
            count++;
        }
    }

    return count;
}

int get_word_count(int len, string text)
{
    int count = 0;

    for (int i = 0; i < len; i++)
    {
        if (isblank(text[i]))
        {
            count++;
        }
    }

    return ++count; // Return with incrementation, word count is 1 more than space count
}

int get_sentence_count(int len, string text)
{
    int count = 0;

    for (int i = 0; i < len; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }

    return count;
}
