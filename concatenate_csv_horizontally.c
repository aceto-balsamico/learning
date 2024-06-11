#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define MAX_COLUMNS 100

void concatenate_csv_horizontally(const char* fileA, const char* fileB, const char* outputFile)
{
	FILE* inputFileA = fopen(fileA, "r");
	FILE* inputFileB = fopen(fileB, "r");
	FILE* outFile = fopen(outputFile, "w");

	if (inputFileA == NULL || inputFileB == NULL || outFile == NULL)
	{
		fprintf(stderr, "Error opening file.\n");
		exit(1);
	}

	char lineA[MAX_LINE_LENGTH] = {};
	char lineB[MAX_LINE_LENGTH] = {};
	char* headersA[MAX_COLUMNS] = {};
	char* headersB[MAX_COLUMNS] = {};
	int columnsA = 0;
	int columnsB = 0;

	// Read and store headers
	fgets(lineA, sizeof(lineA), inputFileA);
	fgets(lineB, sizeof(lineB), inputFileB);

	// Tokenize headers of file A
	char* token = strtok(lineA, ",\n");
	while (token != NULL)
	{
		if (columnsA == MAX_COLUMNS)
		{
			fprintf(stderr, "Error: Too many columns in file A.\n");
			exit(1);
		}
		headersA[columnsA++] = strdup(token);
		token = strtok(NULL, ",\n");
	}

	// Tokenize headers of file B
	token = strtok(lineB, ",\n");
	while (token != NULL)
	{
		if (columnsB == MAX_COLUMNS)
		{
			fprintf(stderr, "Error: Too many columns in file B.\n");
			exit(1);
		}
		headersB[columnsB++] = strdup(token);
		token = strtok(NULL, ",\n");
	}

	// Write new headers to the output file
	for (int i = 0; i < columnsA; i++)
	{
		fprintf(outFile, "%s", headersA[i]);
		if (i < columnsA - 1)
		{
			fprintf(outFile, ",");
		}
	}
	fprintf(outFile, ",");

	for (int i = 0; i < columnsB; i++)
	{
		fprintf(outFile, "%s", headersB[i]);
		if (i < columnsB - 1)
		{
			fprintf(outFile, ",");
		}
	}
	fprintf(outFile, "\n");

	// Read and merge data lines
	bool eofA = false;
	bool eofB = false;
	while (!eofA || !eofB)
	{
		char* dataA[MAX_COLUMNS] = { NULL };
		char* dataB[MAX_COLUMNS] = { NULL };
		int filledA = 0;
		int filledB = 0;

		// Tokenize data line of file A
		if (fgets(lineA, sizeof(lineA), inputFileA))
		{
			token = strtok(lineA, ",\n");
			while (token != NULL)
			{
				dataA[filledA++] = strdup(token);
				token = strtok(NULL, ",\n");
			}
		}
		else
		{
			eofA = true;
		}

		// Tokenize data line of file B
		if (fgets(lineB, sizeof(lineB), inputFileB))
		{
			token = strtok(lineB, ",\n");
			while (token != NULL)
			{
				dataB[filledB++] = strdup(token);
				token = strtok(NULL, ",\n");
			}
		}
		else
		{
			eofB = true;
		}

		// Write data to the output file
		for (int i = 0; i < columnsA; i++)
		{
			if (dataA[i])
			{
				fprintf(outFile, "%s", dataA[i]);
			}
			if (i < columnsA - 1)
			{
				fprintf(outFile, ",");
			}
		}
		fprintf(outFile, ",");

		for (int i = 0; i < columnsB; i++)
		{
			if (dataB[i])
			{
				fprintf(outFile, "%s", dataB[i]);
			}
			if (i < columnsB - 1)
			{
				fprintf(outFile, ",");
			}
		}
		fprintf(outFile, "\n");

		// Free allocated memory for the current line
		for (int i = 0; i < filledA; i++)
		{
			free(dataA[i]);
		}
		for (int i = 0; i < filledB; i++)
		{
			free(dataB[i]);
		}
	}

	// Free allocated memory for headers
	for (int i = 0; i < columnsA; i++)
	{
		free(headersA[i]);
	}
	for (int i = 0; i < columnsB; i++)
	{
		free(headersB[i]);
	}

	fclose(inputFileA);
	fclose(inputFileB);
	fclose(outFile);
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s fileA fileB\n", argv[0]);
		return 1;
	}

	concatenate_csv_horizontally(argv[1], argv[2], "output_horizontal.csv");

	return 0;
}