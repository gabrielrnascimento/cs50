#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    int BLOCK_SIZE = 512;
    int image_number = 0;
    int start_jpg = 0;
    char file_name[8];
    typedef uint8_t BYTE;
    BYTE buffer[BLOCK_SIZE];

    FILE *raw_file = fopen(argv[1], "r");
    FILE *img;

    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            if (start_jpg)
            {
                image_number++;
            }
            else
            {
                start_jpg = 1;
            }
            sprintf(file_name, "%03i.jpg", image_number);
            // printf("%s\n",file_name);
            img = fopen(file_name, "w");
            fwrite(buffer, 1, BLOCK_SIZE, img);
            fclose(img);
        }
        else if (start_jpg)
        {
            sprintf(file_name, "%03i.jpg", image_number);
            img = fopen(file_name, "a");
            fwrite(buffer, 1, BLOCK_SIZE, img);
            fclose(img);
        }
    }
    fclose(raw_file);
}
