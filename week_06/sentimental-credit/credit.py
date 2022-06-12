from cs50 import get_string


def main():
    card_number = get_string("Number: ")
    # card_number = input("Number: ")
    if luhns_algorithm(card_number) == False:
        print("INVALID")
        return 1
    return print(check_card(card_number))


def luhns_algorithm(card_number):
    numbers = 0
    # ? Step 1
    for i in range(len(card_number)-2, -1, -2):
        first_step = int(card_number[i]) * 2
        if first_step >= 10:
            numbers += int(str(first_step)[0]) + int(str(first_step)[1])
        else:
            numbers += first_step
    # ? Step 2
    for i in range(len(card_number[:-1]), -1, -2):
        numbers += int(card_number[i])
    # ? Step 3
    if (numbers % 10) == 0:
        return True
    else:
        return False


def check_card(card_number):
    first_two_digits = int(card_number[0:2])
    # ? Check card for VISA conditions
    if (first_two_digits >= 40 and first_two_digits <= 49) and (len(card_number) == 13 or len(card_number) == 16):
        return "VISA"
    # ? Check card for AMEX conditions
    elif (first_two_digits == 34 or first_two_digits == 37) and len(card_number) == 15:
        return "AMEX"
    # ? Check card for MASTERCARD conditions
    elif (first_two_digits >= 51 and first_two_digits <= 55) and len(card_number) == 16:
        return "MASTERCARD"
    return "INVALID"


main()