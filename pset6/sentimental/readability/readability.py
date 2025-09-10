import re
from cs50 import get_string


def main():
    text = get_string("Text: ")
    print(grade(text))


def grade(text: str) -> str:
    # Count letters (A-Z, a-z)
    letter_count = sum(1 for c in text if re.fullmatch(r"[A-Za-z]", c))

    # Count words (spaces + 1)
    word_count = text.count(" ") + 1

    # Count sentences (. ! ?)
    sentence_count = sum(1 for c in text if c in [".", "!", "?"])

    # Coleman-Liau index
    L = letter_count / word_count * 100
    S = sentence_count / word_count * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)

    if index < 1:
        return "Before Grade 1"
    elif index >= 16:
        return "Grade 16+"

    return f"Grade {index}"


if __name__ == "__main__":
    main()
