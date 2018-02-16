/* Name:       Evan Pringle
 * Class:      EECE4029 / Opr. Systems
 * Assignment: Homework 3 - Part A
 * Due Date:   February 16, 2018
 */

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{

	/*
	 * What else that needs to be completed:
	 * 1) Take all the file names in the directory and store in an array
	 *
	 * 2) Take all the process info (process names and IDs) of the system
	 *    and appends them to the array mentioned above
	 *
	 * 3) Finally, you should send the contents of the array to the email 
	 *    (use UNIX 'mail' or 'mailx')
	 */

	if (argc != 2){
		printf("ERROR: Enter one parameter\n");
	} else if ( atoi(argv[1]) != 0 ) {
		printf("ERROR: Enter an email\n");
	} else {
		printf("Correct!\n");
	}
	

	return 0;
}