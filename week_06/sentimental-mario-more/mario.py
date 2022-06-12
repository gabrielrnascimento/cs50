from cs50 import get_int


def main():
    height = get_height()
    for i in range(height):
        print(left_side(i, height) + middle_space() + right_side(i))


def get_height():
    while True:
        height = get_int("Height: ")
        if height <= 0:
            print("Invalid input! Please type another number")
        elif height > 8:
            print("Invalid input! Please type another number")
        else:
            return height


def left_side(i, height):
    i = i + 1
    spaces = (height - i) * " "
    hashes = i * "#"
    return spaces + hashes


def middle_space():
    return "  "


def right_side(i):
    i = i + 1
    hashes = i * "#"
    return hashes


main()