#include "csv_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define MAX_COLUMNS 100

FILE* create_csv_with_timestamp(const char* base_filename, char* filename, size_t filename_size) {
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];
	// char filename[256];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%Y%m%d_%H%M%S", timeinfo);
	snprintf(filename, filename_size, "%s_%s.csv", base_filename, buffer);

	FILE* fp = fopen(filename, "w");
	
	return fp;
}

char* get_line(FILE* fp, size_t line_number) {
	static char buffer[1024];
	size_t current_line = 0;

	while (fgets(buffer, sizeof(buffer), fp)) {
		if (current_line == line_number) {
			return buffer;
		}
		current_line++;
	}
	return NULL;
}

int get_nth_token_as_int(const char* str, int n) {
	char* token;
	char buffer[1024];
	strncpy(buffer, str, sizeof(buffer));
	token = strtok(buffer, ",");
	for (int i = 0; i < n; i++) {
		if (token == NULL) return -1;
		token = strtok(NULL, ",");
	}
	return token ? atoi(token) : -1;
}

int get_nth_token_as_hex(const char* str, int n) {
	char* token;
	char buffer[1024];
	strncpy(buffer, str, sizeof(buffer));
	token = strtok(buffer, ",");
	for (int i = 0; i < n; i++) {
		if (token == NULL) return -1;
		token = strtok(NULL, ",");
	}
	return token ? strtol(token, NULL, 16) : -1;
}

char* get_nth_token_as_string(const char* str, int n, char* buffer, size_t buffer_size) {
	char* token;
	char temp[1024];
	strncpy(temp, str, sizeof(temp));
	token = strtok(temp, ",");
	for (int i = 0; i < n; i++) {
		if (token == NULL) return NULL;
		token = strtok(NULL, ",");
	}
	if (token) {
		strncpy(buffer, token, buffer_size);
		return buffer;
	}
	return NULL;
}

void extract_column_to_new_csv(const char* input_filename, const char* output_filename, int column_number) {
	FILE* input_fp = fopen(input_filename, "r");
	FILE* output_fp = fopen(output_filename, "w");
	if (!input_fp || !output_fp) {
		perror("File opening failed");
		return;
	}

	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), input_fp)) {
		char token_buffer[1024];
		if (get_nth_token_as_string(buffer, column_number, token_buffer, sizeof(token_buffer))) {
			fprintf(output_fp, "%s\n", token_buffer);
		}
	}

	fclose(input_fp);
	fclose(output_fp);
}

void concatenate_csv_files(const char* file1, const char* file2, const char* output_filename) {
	FILE* fp1 = fopen(file1, "r");
	FILE* fp2 = fopen(file2, "r");
	FILE* output_fp = fopen(output_filename, "w");
	if (!fp1 || !fp2 || !output_fp) {
		perror("File opening failed");
		return;
	}

	char buffer1[1024];
	char buffer2[1024];

	while (fgets(buffer1, sizeof(buffer1), fp1) && fgets(buffer2, sizeof(buffer2), fp2)) {
		buffer1[strcspn(buffer1, "\n")] = '\0'; // Remove newline character
		fprintf(output_fp, "%s,%s", buffer1, buffer2);
	}

	fclose(fp1);
	fclose(fp2);
	fclose(output_fp);
}

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

