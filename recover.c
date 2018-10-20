#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define BLKSIZE 512
#define PHOTOSIZE 2500000 // 1.5 mb big enough for photos
    
    typedef uint8_t  BYTE;
    
    typedef struct
    {
        char space;
        char photo[PHOTOSIZE];
    }
    JPEGFINDER;
    
    typedef struct
    {
        char photo[PHOTOSIZE];
        char* picPtr;
    }
    JPEG;
    
    // signature first bytes of JPEG file format
    const BYTE hex1 = 0xff;
    const BYTE hex2 = 0xd8;
    const BYTE hex3 = 0xff;
    const BYTE hex4 = 0xe0;
    const BYTE filterHex = 0xf0;
    
int main (int argc, char* argv[] )
{
    // usage reminder
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // name for user input file
    char* memories = argv[1];
    // declared variables...
    JPEG* tempo;
    JPEGFINDER* jpgPtr;
    BYTE test[4];
    long p;
    int count = 0;
    size_t byteCount;

    // allocate memory to write to
    tempo = (JPEG*)calloc(1, sizeof(JPEG));
    if (tempo == NULL)
    {
        fprintf(stderr, "could not allocate temp memory");
        return 4;
    }
    
    // name file with no images
    tempo->picPtr = "Junk";
    char*remover = tempo->picPtr;
  
    // allocate memory for read/write buffer
    jpgPtr = (JPEGFINDER *)calloc(1, sizeof(JPEGFINDER));
    if (jpgPtr == NULL)
    {
        fprintf(stderr, "could not allocate memory to jpg_ptr\n");
        return 5;
    }

    // create FILE pointer and open readable file...
    FILE *picdrive = fopen(memories, "r");
    if (picdrive == NULL)
    {
        fprintf(stderr, "Could not open data file %s.\n", argv[1]);
        free(tempo);
        free(jpgPtr);
        return 2;
    }
    // create FILE pointer and open writable file....
    FILE *picstore = fopen(tempo->picPtr, "w");
    if (picstore == NULL)
    {
        fprintf(stderr, "Could not open write file 'junk'.\n");
        fclose(picdrive);
        free(tempo);
        free(jpgPtr);
        return 3;
    }

    // one do/while loop is all we need
    do
    {
        // test first 4 bytes of block for JPEG signature
        for (int i = 0; i < 4; i++)
        {
            fread(&jpgPtr->space, sizeof(BYTE), 1, picdrive);
            test[i] = jpgPtr->space;
        }

        // reset read file to beginning of block
        p = (ftell(picdrive) - 4);
        fseek(picdrive, p, SEEK_SET);

        // test for matching JPEG signature
        if (test[0] == hex1 &&
            test[1] == hex2 &&
            test[2] == hex3 &&
            (test[3] & filterHex) == hex4)
            
        {
            // signature found close Write file and proceed
            fclose(picstore); // creates physical file

            // free allocated memory from Read buffer
            free(jpgPtr);
            
            // re-allocate Read memory for next file
            jpgPtr = (JPEGFINDER *)calloc(1, sizeof(JPEGFINDER));
            if (jpgPtr == NULL)
            {
                printf("could not allocate memory to jpg_ptr");
                return 6;
            }

            // free allocated memory from File Write 
            free(tempo);

            // re-allocate Write memory for next file
            tempo = (JPEG*)calloc(1, sizeof(JPEG));
            if (tempo == NULL)
            {
                fprintf(stderr, "could not allocate temp memory");
                return 7;
            }
            
            // establish naming system for future file output
            sprintf(tempo->photo, "%03i.jpg", count);
            count++;
            
            // open new Write files with larger buffer-size for photos
            picstore = fopen(tempo->photo, "w");
            if (picstore == NULL)
            {
                fprintf(stderr, "Could not open new write file.\n");
                fclose(picdrive);
                free(tempo);
                free(jpgPtr);
                remove(remover);
                return 8;
            }

            // read and write the first BLOCK of data from found jpeg ... 
            byteCount = fread(&jpgPtr->photo, sizeof(BYTE), BLKSIZE, picdrive);
            // check for end of File
            if (byteCount != BLKSIZE)
            {
                //fprintf(stderr, "end of file reached....\n");
                fclose(picdrive);
                fclose(picstore);
                free(tempo);
                free(jpgPtr);
                remove(remover);
                return 0;
            }
            // write first BLOCK of data to new file
            fwrite(&jpgPtr->photo, sizeof(BYTE), BLKSIZE, picstore);
            
        }
        else 
        {
            // read and write additional file data BLOCKS at a time until next JPEG is located... 
            byteCount = fread(&jpgPtr->photo, sizeof(BYTE), BLKSIZE, picdrive);

            if (byteCount != BLKSIZE)
            {
                //printf("end of file reached....here\n");
                fclose(picdrive);
                fclose(picstore);
                free(tempo);
                free(jpgPtr);
                remove(remover);
                return 0;
            }

            fwrite(&jpgPtr->photo, sizeof(BYTE), BLKSIZE, picstore);
        
        }
    }
    // this condition is only met if no JPEG signatures are present on input file
    while (feof(picdrive) == 0);
    //fprintf(stderr, "unexpected end of file reached...");
    fclose(picdrive);
    fclose(picstore);
    free(tempo);
    free(jpgPtr);
    remove(remover);
    return 9;
}
    