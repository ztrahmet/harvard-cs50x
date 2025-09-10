import re
from cs50 import get_string


def main():
    card_number = get_string("Number: ")
    print(card_type(card_number))

# Check card numer with Luhn's Algorithm


def luhn_check(number: str) -> bool:
    try:
        digits = [int(d) for d in number[::-1]]
    except ValueError:
        # Card number must be all digits
        return False
    total = sum(digits[::2]) + sum(sum(divmod(d * 2, 10)) for d in digits[1::2])
    return total % 10 == 0

# Match the regex


def card_type(number: str) -> str:
    if luhn_check(number):
        if re.fullmatch(r"3[47]\d{13}", number):
            return "AMEX"
        if re.fullmatch(r"5[1-5]\d{14}", number):
            return "MASTERCARD"
        if re.fullmatch(r"4\d{12}(\d{3})?", number):
            return "VISA"
    return "INVALID"


if __name__ == "__main__":
    main()
