/*  Code for the user interface for Lab 4 for CS 211 Fall 2015
 *
 *  Author: Pat Troy
 *  Date: 10/6/2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
int debugMode = 0;
typedef enum {FALSE = 0, TRUE, NO = 0, YES} boolean;
typedef struct List
{
    char *name;
    int size;
    char status;
    struct List* next;
}List;
/*function definition*/
List* front;
List* back;
void add_to_list(char* name, int size, char status);
bool does_name_exist(char* name);
void display_list(List* front);
void update_status(char* name);
bool retrieve_and_remove(int size);
int countGroupsAhead(char* name);
void displayGroupSizeAhead(char* name);

int getNextNWSChar ();
int getPosInt ();
char *getName();
void clearToEoln();
void printCommands();
void doAdd ();
void doCallAhead ();
void doWaiting ();
void doRetrieve ();
void doList ();
void doDisplay ();
void clearToEoln();

/* Read in until the first Non-White-Space character is Read */
/* The white space characters are:
 *      space, tab \t, newline \n, vertical tab \v,
 *      form feed \f, and carriage return \r
 */
int getNextNWSChar ()
{
    int ch;
    
    ch = getc(stdin);
    if (ch == EOF || ch == '\n')
        return ch;
    while (isspace (ch))
    {
        ch = getc(stdin);
        if (ch == EOF || ch == '\n')
            return ch;
    }
    return ch;
}

/* read in the next Positive Integer or error:    */
/* This is based on the Mathematical definition of a Postive Integer */
/*    zero is not counted as a positive number */
int getPosInt ()
{
    int value = 0;
    
    /* clear white space characters */
    int ch;
    ch = getc(stdin);
    while (!isdigit(ch))
    {
        if ('\n' == ch)  /* error \n ==> no integer given */
            return 0;
        if (!isspace(ch)) /* error non white space ==> integer not given next */
        {
            clearToEoln();
            return 0;
        }
        ch = getc(stdin);
    }
    
    value = ch - '0';
    ch = getc(stdin);
    while (isdigit(ch))
    {
        value = value * 10 + ch - '0';
        ch = getc(stdin);
    }
    
    ungetc (ch, stdin);  /* put the last read character back in input stream */
    
    /* Integer value of 0 is an error in this program */
    if (0 == value)
        clearToEoln();
    
    return value;
}

/* read in the name until the end of the input */
char *getName()
{
    /* skip over the white space characters */
    int ch;
    ch = getc(stdin);
    while (isspace(ch))
    {
        if ('\n' == ch)  /* error \n ==> no integer given */
            return NULL;
        ch = getc(stdin);
    }
    
    char *word;
    int size;
    size = 10;
    word = (char *) malloc (sizeof(char) * size);
    
    // read in character-by-character until the newline is encountered
    int count = 0;
    
    while (ch != '\n')
    {
        if (count+1 >= size)
        {
            // to grow an array to make it "dynamically sized" using malloc
            char* temp;
            int i;
            size = size * 2;
            temp = (char *) malloc (sizeof(char) * size);
            
            // printf ("Growing array from size %d to size %d\n", count, size);
            // copy the characters to the new array
            for (i = 0 ; i < count ; i++)
                temp[i] = word[i];
            
            free (word);
            word = temp;
        }
        
        word[count] = ch;
        count++;
        word[count] = '\0';
        
        // read next character
        ch = getc(stdin);
    }
    
    if (count > 30)
    {
        count = 30;
        word[count] = '\0';
    }
    
    /* clear ending white space characters */
    while (isspace (word[count-1]))
    {
        count--;
        word[count] = '\0';
    }
    
    return word;
}

/* Clear input until next End of Line Character - \n */
void clearToEoln()
{
    int ch;
    
    do {
        ch = getc(stdin);
    }
    while ((ch != '\n') && (ch != EOF));
}

/* Print out a list of the commands for this program */
void printCommands()
{
    printf ("The commands for this program are:\n\n");
    printf ("q - to quit the program\n");
    printf ("? - to list the accepted commands\n");
    printf ("a <size> <name> - to add a group to the wait list\n");
    printf ("c <size> <name> - to add a call-ahead group to the wait list\n");
    printf ("w <name> - to specify a call-ahead group is now waiting in the restaurant\n");
    printf ("r <table-size> - to retrieve the first waiting group that can fit at the available table size\n");
    printf ("l <name> - list how many groups are ahead of the named group\n");
    printf ("d - display the wait list information\n");
    
    /* clear input to End of Line */
    clearToEoln();
}

void doAdd ()
{
    /* get group size from input */
    int size = getPosInt();
    if (size < 1)
    {
        printf ("Error: Add command requires an integer value of at least 1\n");
        printf ("Add command is of form: a <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    /* get group name from input */
    char *name = getName();
    if (NULL == name)
    {
        printf ("Error: Add command requires a name to be given\n");
        printf ("Add command is of form: a <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    printf ("Adding In-restaurant group \"%s\" of size %d\n", name, size);
    char status = 'a';
    // add code to perform this operation here
    if(does_name_exist(name) == true)//check if the name is exist
    {
        printf("Error: name is already in the list.\n");
    }else
    {
        add_to_list(name, size, status);//add group to the like
    }
}


void doCallAhead ()
{
    /* get group size from input */
    int size = getPosInt();
    if (size < 1)
    {
        printf ("Error: Call-ahead command requires an integer value of at least 1\n");
        printf ("Call-ahead command is of form: c <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    /* get group name from input */
    char *name = getName();
    if (NULL == name)
    {
        printf ("Error: Call-ahead command requires a name to be given\n");
        printf ("Call-ahead command is of form: c <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    printf ("Adding Call-ahead group \"%s\" of size %d\n", name, size);
    
    // add code to perform this operation here
    int status = 'c';
    if(does_name_exist(name) == true)//check if the name is exist
    {
        printf("Error: name is already in the list.\n");
    }else
    {
        add_to_list(name, size, status);//add group to the list
    }
}
void doWaiting ()
{
    /* get group name from input */
    char *name = getName();
    if (NULL == name)
    {
        printf ("Error: Waiting command requires a name to be given\n");
        printf ("Waiting command is of form: w <name>\n");
        printf ("  where: <name> is the name of the group that is now waiting\n");
        return;
    }
    
    printf ("Call-ahead group \"%s\" is now waiting in the restaurant\n", name);
    
    // add code to perform this operation here
    update_status(name);//change the status to in-restaurant waiting
}
void doRetrieve ()
{
    /* get table size from input */
    int size = getPosInt();
    if (size < 1)
    {
        printf ("Error: Retrieve command requires an integer value of at least 1\n");
        printf ("Retrieve command is of form: r <size>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        return;
    }
    clearToEoln();
    printf ("Retrieve (and remove) the first group that can fit at a tabel of size %d\n", size);
    
    // add code to perform this operation here
    //check if there is a group can fit in the table, if true remove the group from the list
    if(retrieve_and_remove(size)== false)
    {
        printf("Error: No group is waiting in the restaurant can fit in that table.\n");
    }
}
void doList ()
{
    /* get group name from input */
    char *name = getName();
    if (NULL == name)
    {
        printf ("Error: List command requires a name to be given\n");
        printf ("List command is of form: l <name>\n");
        printf ("  where: <name> is the name of the group to inquire about\n");
        return;
    }
    
    printf ("Group \"%s\" is behind the following groups\n", name);
    
    // add code to perform this operation here
    int num_group_ahead;
    if(does_name_exist(name) == true)//check if the name is exist
    {
        num_group_ahead = countGroupsAhead(name);//num_group_ahead equal to the return value of the function
        printf("There are %d group ahead %s.\n", num_group_ahead, name);
        displayGroupSizeAhead(name);//print out the size of each group ahead target name
    }else
    {
        printf("Error: name is not in the waiting list.\n");
    }
}
void doDisplay ()
{
    clearToEoln();
    printf ("Display information about all groups\n");
    
    // add code to perform this operation here
    display_list(front);//print out the whole list
}


int main (int argc, char **argv)
{
    
    char *input;
    int ch;
    
    printf ("Starting Restaurant Wait List Program\n\n");
    printf ("Enter command: ");
    
    while ((ch = getNextNWSChar ()) != EOF)
    {
        /* check for the various commands */
        if ('q' == ch)
        {
            printf ("Quitting Program\n");
            return (0);
        }
        else if ('?' == ch)
        {
            printCommands();
        }
        else if('a' == ch)
        {
            doAdd();
        }
        else if('c' == ch)
        {
            doCallAhead();
        }
        else if('w' == ch)
        {
            doWaiting();
        }
        else if('r' == ch)
        {
            doRetrieve();
        }
        else if('l' == ch)
        {
            doList();
        }
        else if('d' == ch)
        {
            doDisplay();
        }
        else if('\n' == ch)
        {
            /* nothing entered on input line     *
             * do nothing, but don't give error  */
        }
        else
        {
            printf ("%c - in not a valid command\n", ch);
            printf ("For a list of valid commands, type ?\n");
            clearToEoln();
        }
        
        printf ("\nEnter command: ");
    }
    
    printf ("Quiting Program - EOF reached\n");
    return 1;
}

void add_to_list(char* name, int size, char status)
{
    List* tmp = (List*)malloc(sizeof(List));
    tmp->name = (char*)malloc(sizeof(char));
    strcpy(tmp->name, name);
    tmp->size = size;
    tmp->status = status;
    
    //if list is empty, add one group to the list, since there is only one group front equals back
    if(front == NULL)
    {
        front = back = tmp;
        back->next = NULL;
        printf("Group %s is added to the waiting list",tmp->name);
    }else//list is not empty, push group to the back
    {
        back->next = tmp;
        back = tmp;
        back->next = NULL;
        printf("Group %s is added to the waiting list",tmp->name);
    }
    if ( debugMode == TRUE )
    {
        printf (" Debugging Information \n");
        printf("Name: %s, Size: %d, Status: %c\n",tmp->name, tmp->size, tmp->status);
    }
}

bool does_name_exist(char* name)
{
    List* tmp = front;
    while(tmp != NULL)//go through the whole list
    {
        if(!strcmp(name, tmp->name))//find the target name in the list
        {
            return true;
        }
        tmp = tmp->next;
        if ( debugMode == TRUE )
        {
            printf (" Debugging Information \n");
            printf("Name: %s, Size: %d, Status: %c\n",tmp->name, tmp->size, tmp->status);
        }
    }
    return false;//if name is not found in the list, return false
}

void update_status(char* name)
{
    List* tmp = front;
    if(does_name_exist(name) == true)//check if the name is in the list
    {
        while(tmp != NULL)//go through the whole list
        {
            if(!strcmp(name, tmp->name))//find the target name in the list
            {
                tmp->status = 'a';//change the status to in-restaurant waiting
            }
            tmp = tmp->next;
        }
    }else
    {
        printf("Error: name is not in the waiting list.\n");
    }
}

bool retrieve_and_remove(int size)
{
    List* tmp, *curr = front;
    if(front == NULL)//list is empty
    {
        if ( debugMode == TRUE )
        {
            printf (" Debugging Information \n");
            printf("Name: %s, Size: %d, Status: %c",front->name, front->size, front->status);
        }
        return false;
    }
    else if(front->size <= size && front->status == 'a')//check first group size and status
    {
        //delete first group from the list
        tmp = front;
        front = front->next;
        if ( debugMode == TRUE )
        {
            printf (" Debugging Information \n");
            printf("Name: %s, Size: %d, Status: %c",front->name, front->size, front->status);
        }
        return true;
    }
    while(curr ->next!= NULL)//more than one group is in the list
    {
        if(curr->next->size <= size && curr->next->status == 'a')//check size and status
        {
            printf("Group %s is seated now.\n", curr->next->name);
            //delete the group from the list
            tmp = curr->next;
            curr->next = tmp->next;
            return true;
        }
        curr = curr->next;
        if ( debugMode == TRUE )
        {
            printf (" Debugging Information \n");
            printf("Name: %s, Size: %d, Status: %c",curr->name, curr->size, curr->status);
        }
    }
    return false;
}

int countGroupsAhead(char* name)
{
    int number = 0;
    List* tmp = front;
    if(does_name_exist(name) == true)//check if the name is in the list
    {
        while(tmp != NULL)//go through the whole list
        {
            if(!strcmp(name, tmp->name))//stop when find the target name
            {
                break;
            }
            number++;//count how many group are ahead
            tmp = tmp->next;
        }
    }
    return number;
}
void displayGroupSizeAhead(char* name)
{
    List* tmp = front;
    if(does_name_exist(name) == true)//check if the name is in the list
    {
        printf("Size: ");
        while(tmp != NULL)//go through the whole list
        {
            if(!strcmp(name, tmp->name))//stop when find the target name
            {
                break;
            }
            printf("%d ",tmp->size);//print out group size
            tmp = tmp->next;
        }
    }
}
void display_list(List* front)
{
    List* tmp = front;
    int i=0;
    if(tmp == NULL)//list is empty
    {
        printf("No group are in the waiting list.\n");
    }
    while(tmp != NULL)//go through the whole list
    {
        printf("Name: %s\n", tmp->name);//print out name
        printf("Size: %d\n", tmp->size);//print out size
        //print out status
        printf("status: ");
        if(tmp->status == 'a')
        {
            printf("in-restaurant wating\n\n");
        }
        if(tmp->status == 'c')
        {
            printf("call ahead\n\n");
        }
        tmp = tmp->next;
        i++;//count how many group in total
        if ( debugMode == TRUE )
        {
            printf (" Debugging Information \n");
            printf("Name: %s, Size: %d, Status: %c",tmp->name, tmp->size, tmp->status);
        }
    }
    printf("Total of %d groups.\n\n", i);//print out total
}






