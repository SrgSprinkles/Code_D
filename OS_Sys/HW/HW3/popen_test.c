/* Name:       Evan Pringle
 * Class:      EECE4029 / Opr. Systems
 * Assignment: Homework 3 - Part A
 * Due Date:   February 16, 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

	//Checks if there is one argument
	if (argc != 2) {
		perror("argument");
		exit(-1);
	}

	const int PATH_MAX = 100;
	char path[PATH_MAX];					// Information array, 100 entries
	
	/* First part that calls "ls | sort" and stores it into path */
	FILE *fp_file_names;						// Declare file to call "ls"

	fp_file_names = popen("ls * | sort", "r");	// Calls ls | sort in terminal
	if (fp_file_names == NULL)					// Checks error in fp
	{
		perror("Cannot start 'ls'\n");
		exit(-1);
	}

	while (fgets(path, PATH_MAX, fp_file_names) != NULL)
	{
		printf("%s", path);
	}

	if (pclose(fp_file_names) != 0)
	{
		fprintf(stderr, "Could not close file because of errors.\n");
	}

	/* Second part that calls "ps -e" and appends it onto path */
	FILE *fp_pid;
	
	fp_pid = popen("ps -e", "r");
	if (fp_pid == NULL)
	{
		perror("Cannot start 'ps'\n");
		exit(-1);
	}

	while (fgets(path, PATH_MAX, fp_pid) != NULL)
	{
		printf("%s", path);
	}
	
	if (pclose(fp_pid) != 0)
	{
		fprintf(stderr, "Could not close file because of errors.\n");
	}

	/* Third part that calls "mail" and sends an email */
	execl("/bin/mail", "", "evan.pringle1@gmail.com", NULL);

	return 0;
}