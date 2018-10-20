#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main (void)
{
   string userName;
   int nameLength, i = 0;
   
    // Ask user for input ...commented out as it is not part of the expected "test" output...
    //printf ("Enter a full name: ");
    
    userName = get_string();  
    
    /*  for safety purposes....this conditional verifies the user input is not NULL...
        ...(honestly?....I'm not completely certain what that entails ...just yet...)... */
    if (userName != NULL)
    {
            nameLength = strlen(userName);
    
        /*  scan for first non-blank-space input and print capitalized first initial...
           ...a while loop here allows the program to break once the condition is met...  */
       while (i < nameLength)
       {
            if (userName[i] != ' ')
            {
                printf ("%c", toupper(userName[i]));
                break;
            }
            else
            {
                i++;
            }
       }
       
       /*  leaving i where it was after the previous conditional...
            ...continue looping through the characters of the userName...
            ....specifically searching for letters that follow a space....
            ....then capitalizing them upon output....  */
       for (; i < nameLength - 1; i++)
       {
           if (userName[i] == ' ' && userName[i+1] != ' ')
           {
               printf ("%c", toupper(userName[i+1]));
           }
       }
    }
    
    printf ("\n");
    
        return 0;
}