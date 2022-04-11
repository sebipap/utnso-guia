#include "commons/txt.h"
#include "commons/string.h"
#include "commons/collections/list.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	char* region;
	char *fullName;
	int age;
	int phone;
	int dni;
	int saldo;
} Person;

char *char_to_string(char);
int times(char, char *);

int main(void)
{

	FILE *inputFile = fopen("personas.txt", "r");

	char *fileString = string_new();

	while (!feof(inputFile))
		string_append(&fileString, char_to_string(fgetc(inputFile)));

	fclose(inputFile);

	int peopleAmount = times('\n', fileString) + 1;

	char **lines = string_n_split(fileString, peopleAmount, "\n");

	t_list *people = list_create();

	for (int i = 0; i < peopleAmount; i++)
	{
		char **fields = string_n_split(lines[i], 6, "; ");
		Person *person = malloc(sizeof(Person));

		person -> region = fields[0];
		person -> fullName = fields[1];
		person -> age = atoi(fields[2]);
		person -> phone = atoi(fields[3]);
		person -> dni = atoi(fields[4]);
		person -> saldo = atoi(fields[5]);

		list_add(people, person);
	}

	//FILE* outputFile = txt_open_for_append("output.txt");

	for (int i = 0; i < peopleAmount; i++)
	{
		Person *person = list_get(people, i);
		char* outputString = string_from_format("%s | %d | %d | %.30s | %d \n", person->region, person->age, person->dni, person->fullName, person->phone);
			
		if(person->age >= 18)
			printf("%s", outputString)
			//txt_write_in_file(outputFile, outputString);
	}

	return 0;
}

char *char_to_string(char c)
{
	char *tmp = malloc(sizeof(char) * 2);
	sprintf(tmp, "%c", c);

	return tmp;
}

int times(char character, char *string)
{
	int times = 0;

	for (int i = 0; i < string_length(string); i++)
		if (string[i] == character)
			times++;

	return times;
}