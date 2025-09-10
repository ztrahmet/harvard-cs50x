#include <cs50.h>
#include <stdio.h>

bool is_valid_card(long);

int main(void)
{
    const long card_number = get_long("Number: ");
    is_valid_card(card_number);
}

bool is_valid_card(long number)
// Declaring function to return true for valid and false for invalid has no effect for this project
{
    int length = 0, // Length of the number
        total = 0,  // Luhn's total of the numbers
        first_two,  // Stores first two digits of the number
        first;      // Stores the first digit of the number

    for (;number != 0; length++)
    {
        int digit = number % 10; // Take the last digit of remaining number
        first = digit;
        first_two = (number > 9 && number < 100) ? // If two digits left assign them as first_two
            number : first_two;
        if (length % 2 == 1) // Every other digit
        {
            digit *= 2; // Multiply by 2
            if (digit > 9)
            {
                digit = (digit / 10) + (digit % 10);
            }
        }
        number /= 10; // Delete the last digit for the next tour
        total += digit;
    }
    if (total % 10 == 0)
    {
        if (length == 15 && (first_two == 34 || first_two == 37))
        {
            printf("AMEX\n");
        }
        else if (length == 16 && first_two >= 51 && first_two <= 55)
        {
            printf("MASTERCARD\n");
        }
        else if ((length == 13 || length == 16) && first == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
            return false;
        }
    }
    else
    {
        // If the last digit of total is not 0, card is invalid
        printf("INVALID\n");
        return false;
    }

    return true; // If not false
}
