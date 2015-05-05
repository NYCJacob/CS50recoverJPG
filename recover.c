/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 *
 * By Jacob Sherman jacob@jbsheman.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


int main(void)
{   
    // open file file io function
    
    FILE* inptr = fopen("card.raw", "r");
        
    // check for successful open
    if (inptr == NULL)
    {
        printf("Could not open file.");
        return 2;
    }
    
    //  create buffer array
    // typedef uint8_t  BYTE;  from bmp.h
    typedef uint8_t buffer[512];

    
    //initialize  new buffer variable
    buffer currentBlock;
    
    int jpgFound = 0;   //  counter of jpgs found
    char fileName[sizeof "123.jpg"];  // array of char for file name
    FILE* fp;
    bool newJpg;  // tracks if new file needed
    
    // while loop loops through raw file 512 bytes at a time
    while (fread(currentBlock, 512, 1, inptr) == 1)
    {
        // checks for jpg signature
        if ( (currentBlock[0] == 0xff) && (currentBlock[1] == 0xd8) && (currentBlock[2] == 0xff) && (currentBlock[3] == 0xe0 || currentBlock[3] == 0xe1) )
        {
            
            // printf("Found a jpg! number %i\n", jpgFound);
            // tracks if new jpg
            newJpg = true;
            // jpgFound to track if any already found and if so how many for file names
            if (jpgFound == 0)
            {
                sprintf(fileName, "%03d.jpg", jpgFound);
                fp = fopen(fileName, "w");
                
                if (fp == NULL)
                {
                    printf("Could not create %s\n", fileName);
                    return 2;
                }
                
                fwrite(currentBlock, 512, 1, fp);
                
            }
            //  if not first jpg found
            else if (jpgFound > 0)
            {
                fclose(fp);
                sprintf(fileName, "%03d.jpg", jpgFound);
                fp = fopen(fileName, "w");
                fwrite(currentBlock, 512, 1, fp);
            }

           jpgFound++; 
          
        }
        else if (jpgFound > 0)
        {
        newJpg = false;
        fwrite(currentBlock, 512, 1, fp);
        }
     
    }
    // close input and output streams
    fclose(fp);
    fclose(inptr);
    return 0;   
}
