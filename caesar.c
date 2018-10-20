#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc, string argv[])
{
    string plain;
    int key, plainLength, alphaIndexer, i;

    
    /*  print error message if command-line input is not 2 arguments in length...
        ...otherwise convert command-line argument to integer value "key" with atoi() function ... */
    if (argc != 2)
    {
        printf ("Usage: ./caesar k\n");
        return 1;
    }
    else
    {
        key = atoi(argv[1]);
    }
    
    /*  prompt user for a string of plaintext ...
       ...for safety make certain input doesn't return NULL ... */
    printf ("plaintext: ");
    plain = get_string();
    
    if (plain != NULL)
    {
        /*  assign length of string input to variable plainLength...
            ...use new variable as length for new char array "cipher"....*/
        plainLength = strlen(plain);
        char cipher[plainLength];
        
        /*  loop through each char of plaintext from user...
          ...check to see if input character is alphabetical and then for upper case ....*/
        for (i = 0; i < plainLength; i++)
        {
            if (isalpha(plain[i]))
            {
                //  set alphaIndexer variable based on capitalization (case) of input....
                if (isupper(plain[i]))
                    { alphaIndexer = 65; }
                else
                    { alphaIndexer = 97; }
                
                /*  caesar cipher formula [Ci = Pi + K) % 26]  ...is performed on input ...
                   ...alphaIndexer modifies value of plain char (Pi)...
                   ...ASCii to zero-based Alpha-Index...for sake of formula...
                   ...and is added back in to create new cipher index value (Ci)....  */    
                cipher[i] = (((plain[i] - alphaIndexer) + key) % 26 + alphaIndexer);
            }
            // if original input was not alphabetical...Cipher index gets Plain index value...
            else
                cipher[i] = plain[i];
        }
     
        printf ("ciphertext: ");
        
        //  display each character of cipher index...
        for (i = 0; i < plainLength; i++)
        {
            printf ("%c", cipher[i]);
        }
    }
    printf ("\n");
    
        return 0;
}
