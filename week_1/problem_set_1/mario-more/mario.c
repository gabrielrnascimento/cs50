// .......#..#
// ......##..##
// .....###..###
// ....####..####
// ...#####..#####
// ..######..######
// .#######..#######
// ########..########

#include <cs50.h>
#include <stdio.h>

void middleSpace()
{
    for (int i = 0; i < 2; i++)
    {
        printf(" ");
    }
}

void hash(row)
{
    for (int i = 0; i < row; i++)
    {
        printf("#");
    }
}

void leftSpace(space_number)
{
    for (int i = 0; i < space_number; i++)
    {
        printf(" ");
    }
}


int main(void)
{
    int size = 0;

    do
    {
        size = get_int("Height: ");
    }
    while (size <= 0 || size >= 9);

    int spaces = size - 1;

    for (int i = 1; i <= size; i++)
    {
        leftSpace(spaces);
        hash(i);
        middleSpace();
        hash(i);
        printf("\n");
        spaces = spaces - 1;
    }

}

