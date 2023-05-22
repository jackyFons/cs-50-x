from cs50 import get_int


def main():
    # gets card number
    card_num = get_int("Number: ")

    # checks if number satisfies luhns algorithm and has correct starting numbers
    if luhns(card_num):
        starting_numbers(card_num)
    else:
        print("INVALID")


# returns length of card number
def card_length(num):
    # counter
    length = 0
    while(num > 0):
        num //= 10
        length += 1
    return length


def luhns(num):
    total = 0
    while num != 0:
        # adds every other digit as is
        total += num % 10
        num //= 10

        # mult every other digit by 2
        mult = (num % 10) * 2
        if mult > 9:
            total += mult % 10
            total += mult // 10
        else:
            total += mult
        num //= 10

    # returns whether algorithm ends in 0
    if total % 10 == 0:
        return True
    else:
        return False


# checks whether card number satisfies length and starting numbers
def starting_numbers(num):
    # gets card length
    length = card_length(num)

    # AMEX is length 15, with starting numbers 34 or 37
    if length == 15:
        num //= 10000000000000
        if num == 34 or num == 37:
            print("AMEX")
            return
    # VISA can be length 13, with starting number 4
    elif length == 13:
        num //= 1000000000000
        if num == 4:
            print("VISA")
            return
    # VISA and MC be length 13
    elif length == 16:
        v_num = num // 1000000000000000
        mc_num = num // 100000000000000
        if v_num == 4:
            print("VISA")
            return
        # MC cards start with numbers 50 through 56
        elif mc_num in range(50, 57):
            print("MASTERCARD")
            return
    # Length does not match any card
    else:
        print("INVALID")
    return


main()
