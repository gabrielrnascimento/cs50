//? American Express
//* 15-digit numbers
//* All numbers start with 34 or 37

//? MasterCard
//* 16-digits
//* Most numbers start with 51, 52, 53, 54 or 55

//? VISA
//* 13-digits and 16-digits
//* All numbers start with 4

// ? Checksum
//* 1. Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
//* 2. Add the sum to the sum of the digits that weren’t multiplied by 2.
//* 3 If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!

#include <cs50.h>
#include <stdio.h>
#include <math.h>

int cardLength(long long number)
{
    long long n = number;
    int counter = 0;
    while (n != 0)
    {
        n = n / 10;
        counter++;
    }
    return counter;
}

int digitFinder(long long number, int card_length, int digit_position)
{
    //? find the nth number
    for (int i = 0; i < digit_position - 1; i++)
    {
        number = number / 10;
    }
    return number % 10;
}

int luhnsAlgorithm(long long number, int card_length)
{
    // ? Step 1
    int first_step = 0;
    for (int i = 1; i <= card_length; i++)
    {
        if (i % 2 == 0)
        {
            int digit = digitFinder(number, card_length, i);
            int digit_doubled = digit * 2;
            if (digit_doubled >= 10)
            {
                for (int j = 1; j <= 2; j++)
                {
                    int digit_part = digitFinder(digit_doubled, 2, j);
                    first_step = first_step + digit_part;
                }
            }
            else
            {
                first_step = first_step + digit_doubled;
            }
        }
    }
    // ? Step 2
    int second_step = 0;
    for (int i = 1; i <= card_length; i++)
    {
        if (i % 2 != 0)
        {
            int digit = digitFinder(number, card_length, i);
            second_step = second_step + digit;
        }
    }
    int sum_result = second_step + first_step;
    // ? Step 3
    if (digitFinder(sum_result, cardLength(sum_result), 1) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string checkCard(int first_two_digits, int card_length)
{
    // ! Check card for VISA conditions
    if ((first_two_digits >= 40 && first_two_digits <= 49) && (card_length == 13 || card_length == 16))
    {
        return "VISA\n";
    }
    // ! Check card for AMEX conditions
    else if ((first_two_digits == 34 || first_two_digits == 37) && card_length == 15)
    {
        return "AMEX\n";
    }
    // ! Check card for MASTERCARD conditions
    else if ((first_two_digits >= 51 && first_two_digits <= 55) && card_length == 16)
    {
        return "MASTERCARD\n";
    }
    return "INVALID\n";
}


int main(void)
{
    long long number = get_long("Number: ");

    int card_length = cardLength(number);
    long double divisor = powl(10, (card_length - 2));
    int first_two_digits = number / divisor;

    // ! Luhn's Algorithm
    bool checksum = luhnsAlgorithm(number, card_length);

    // ! check card
    if (checksum == true)
    {
        string card = checkCard(first_two_digits, card_length);
        printf("%s", card);
    }
    else
    {
        printf("INVALID\n");
    }
}