#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;
const int SIZE = 512;

int main(int argc, char *argv[])
{
    //error if there is 0 or more than 1 argument
    if (argc != 2)
    {

        return 1;
    }

    //Opens file
    FILE *inFile = fopen(argv[1], "r");
    if (inFile == NULL)
    {
        return 1;
    }

    bool found = false;
    int jpeg_counter = 0;
    char file_name[8];
    BYTE buffer[SIZE];
    FILE *outFile;

    //Reads memory
    while (fread(&buffer, SIZE, 1, inFile) == 1)
    {
        /*checks if first 3 bytes are 0xff, 0xd8, 0xff
        AND if 4th byte is either 0xe0, 0xe1, 0xe2, 0xe3, etc.*/
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //closes outgoing file if its not the first jpeg found
            if (found)
            {
                fclose(outFile);
            }
            else
            {
                found = true;
            }

            //create file
            sprintf(file_name, "%03i.jpg", jpeg_counter);
            outFile = fopen(file_name, "w");
            jpeg_counter++;

        }
        //write to file
        if (found)
        {
            fwrite(&buffer, SIZE, 1, outFile);
        }
    }

    //close files
    fclose(inFile);
    fclose(outFile);
}