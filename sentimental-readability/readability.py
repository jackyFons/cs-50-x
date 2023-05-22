from cs50 import get_string


punctuation = [".", "!", "?"]


def main():
    # gets string from user input
    s = get_string("Text: ")

    # calculate index
    words = count_words(s)

    avg_letters = count_letters(s) / words * 100
    avg_sentences = count_sentences(s) / words * 100

    index = round(0.0588 * avg_letters - 0.296 * avg_sentences - 15.8)

    # prints grade
    output(index)

    return


# calculate amount of letters in string
def count_letters(s):
    count = 0
    for char in s:
        if char.isalpha():
            count += 1
    return count


# calculate amount of words in string
def count_words(s):
    count = 0
    for char in s:
        if char == " ":
            count += 1
    return count + 1


# calculate amount of sentences in string
def count_sentences(s):
    count = 0
    for char in s:
        if char in punctuation:
            count += 1
    return count


# determines what output to give
def output(index):
    if index < 1:
        print("Before Grade 1")
    elif index > 15:
        print("Grade 16+")
    else:
        print(f"Grade {index}")
    return


main()