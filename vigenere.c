#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main (int argc, string argv[])
{
    if ( argc != 2 )
    {
        printf ("Usage: ./vigenere k\n");
        return 1;
    }
    
    /*  after making certain that 2 and only 2 arguments are passed at the command line...
        ...some variables are declared...and some of those are initialized...
        ...a char array named key will hold the modified user input from the command-line...  */
    string plain;
    int i, j = 0, alphaIndexer, plainLength, keyLength = strlen(argv[1]);
    char key[keyLength];

    /*  the following loop serves a dual purpose....
     ...first... if any character from the user's command-line "key" input is not alphabetic...
     ... the program returns an error message...
     ...else....the user input is moved into the key array and formatted for later use... */
    for (i = 0; i < keyLength; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf ("Usage: ./vigenere k\n");
            return 1;
        }
        else
        {
            key[i] = (toupper(argv[1][i]) - 65);
        }
    }
    /*  prompt user for a string of plaintext ...
       ...for safety make certain input doesn't return NULL ... */
   printf ("plaintext: ");
   plain = get_string();
   
   if (plain != NULL)
   {
        /*  assign length of string input to variable plainLength...
            ...use new variable as length for new char array "cipher".... */
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
               
                /*  vigenere cipher formula [Ci = Pi + Kj) % 26]  ...is performed on input ...
                   ...alphaIndexer modifies value of plain char (Pi)...
                   ...ASCii to zero-based Alpha-Index...for sake of formula...
                   ...Kj....the key index is incremented with each new alphabetical Pi (PlainText index)...
                   ...the modulo operator is used with keyLength to limit the values to those of the "key"...
                   ...finally, alphaIndexer is added back in with the result being the new cipher index value (Ci)....  */ 
               cipher[i] = ((((plain[i] - alphaIndexer) + key[j % keyLength]) % 26) + alphaIndexer);
               j++;
           }
           // if original input was not alphabetical...Cipher index gets Plain index value...
           else
           {
               cipher[i] = plain[i];
           }
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