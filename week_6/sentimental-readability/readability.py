from cs50 import get_string


def main():
    text = get_string("Text: ")
    letter_count = count_letters(text)
    word_count = count_words(text)
    sentence_count = count_sentences(text)
    index = formula(letter_count, word_count, sentence_count)

# ? return grade
    if index < 1:
        print("Before Grade 1")
    elif index >= 1 and index <= 15:
        print(f"Grade {index}")
    else:
        print(f"Grade 16+")


# ? return the count of letters (or numbers) for the entire string
def count_letters(text):
    letter_count = 0
    for char in text:
        if char.isalnum():
            letter_count += 1
    return letter_count


def count_words(text):
    word_count = 1
    for char in text:
        if char.isspace():
            word_count += 1
    return word_count


def count_sentences(text):
    sentence_count = 0
    for char in text:
        if char in [".", "!", "?"]:
            sentence_count += 1
    return sentence_count
    

def formula(letter_count, word_count, sentence_count):
    L = (letter_count * 100) / word_count
    S = (sentence_count * 100) / word_count
    return round(0.0588 * L - 0.296 * S - 15.8)


main()