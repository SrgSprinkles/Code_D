#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

	//Initial check for arguments
	if (argc != 2) {
		perror("Only one email argument.");
		exit(1);
	}

	int numFiles = 0;
	int numProcs = 0;
	int link[2];

	char listChars[4096];
	char procChars[4096];
	vector<char*> homeworkElements;

	pid_t pid;

	// Fork
	pipe(link);
	pid = fork();

	// If child
	if (pid == 0)
	{
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		execl("/bin/ls", "ls", (char *)0);
	} 
	else 
	{
		close(link[1]);
		int nbytes = read(link[0], listChars, sizeof(listChars));
		char* fileName = strtok(listChars, "\n");

		while (fileName != NULL)
		{
			homeworkElements.push_back(fileName);
			fileName = strtok(NULL, "\n");
		}

		numFiles = homeworkElements.size();
		wait(NULL);
	}

	// Fork
	pipe(link);
	pid = fork();

	// If child
	if (pid == 0) {
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		execl("/bin/ps", "ps", "-e", (char *)NULL);
	}
	else
	{
		close(link[1]);
		int nbytes = read(link[0], procChars, sizeof(procChars));
		char* procInfo = strtok(procChars, "\n");

		while (procInfo != NULL)
		{
			homeworkElements.push_back(procInfo);
			procInfo = strtok(NULL, "\n");
		}

		numProcs = homeworkElements.size() - numFiles - 1;
		wait(NULL);
	}

	// Create output.dat file to write to
	ofstream outputFile;
	outputFile.open("./output.dat");

	// Write to outputFile
	for (int i = 0; i < homeworkElements.size(); i++)
	{
		outputFile << homeworkElements[i] << "\n";
	}

	stringstream convertOne;
	convertOne << numFiles;
	string numFileStr = convertOne.str();
	
	stringstream convertTwo;
	convertTwo << numProcs;
	string numProcsStr = convertTwo.str();
	
	// Hardcoded subject line
	string subject = "There are " + numFileStr + " files and " + numProcsStr + " processes.";

	// Read from output.dat
	std::ifstream ifs("output.dat");
	std::string msg((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	char command[1024];
	sprintf(command, "mail -s '%s' '%s'", subject.c_str(), argv[1]);

	FILE *fp_mail = popen(command, "w");

	// Send an error if fp_mail isn't opened correctly
	if (fp_mail == NULL)
	{
		perror("Failed to send mail.");
		exit(1);
	}

	// Compile whole email
	fprintf(fp_mail, "To: %s\n", argv[1]);
	fprintf(fp_mail, "Subject: %s\n\n", subject.c_str());
	fwrite(msg.c_str(), 1, strlen(msg.c_str()), fp_mail);
	fwrite(".\n", 1, 2, fp_mail);

	// Try and close fp_mail and check for errors
	if (pclose(fp_mail) != 0)
	{
		fprintf(stderr, "Could not close file because of errors.\n");
	}

	// Close outputFile.dat
	outputFile.close();

	return 0;
}