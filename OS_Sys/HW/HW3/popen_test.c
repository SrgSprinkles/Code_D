/* Name:       Evan Pringle
 * Class:      EECE4029 / Opr. Systems
 * Assignment: Homework 3 - Part A
 * Due Date:   February 16, 2018
 */

#include <stdlib.h>
#include <stdio.h>

int main(void)
{

	const int PATH_MAX = 100;
	FILE *fp;
	int status;
	char path[PATH_MAX];

	fp = popen("ls * | sort", "r");
	if (fp == NULL)
	{
		perror("Cannot start ls\n");
		exit(-1);
	}

	while (fgets(path, PATH_MAX, fp) != NULL)
		printf("%s", path);

	if (pclose(fp) != 0)
	{
		fprintf(stderr, "Could not run more or other error.\n");
	}

	return EXIT_SUCCESS;
}