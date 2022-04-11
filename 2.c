#include "commons/string.h"
#include "commons/txt.h"
#include "commons/collections/list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  char *region;
  char *fullName;
  int age;
  char *phoneNumber;
  char *id;
  long balance;
} Person;

void removeLastLineFeed(char *);
char *getLine(FILE *);
int occurrencesOfChar(char *, char);
int fieldsAmount(char *);
char **getFields(char *);
void fillPersonWithFields(Person *, char **);
void initPerson(Person *);
char *personToString(Person);

int main()
{

  FILE *inputFile = fopen("Personas.txt", "r");

  while (!feof(inputFile))
  {
    char *line = getLine(inputFile);
    char **personFields = getFields(line);

    Person person;
    initPerson(&person);
    fillPersonWithFields(&person, personFields);
    puts(personToString(person));
  }

  txt_close_file(inputFile);

  return 0;
}

void removeLastLineFeed(char *text)
{
  if (text[string_length(text) - 1] == '\n')
    text[string_length(text) - 2] = '\0';
}

char *getLine(FILE *file)
{
  char *line = string_new();
  int bufSize = 0;
  int lineSize = getline(&line, &bufSize, file);

  removeLastLineFeed(line);

  return line;
}

int occurrencesOfChar(char *string, char character)
{
  int counter = 0;

  for (int i = 0; i < string_length(string); i++)
    if (string[i] == character)
      counter++;

  return counter;
}

int fieldsAmount(char *string)
{
  return occurrencesOfChar(string, ';') + 1;
}

char **getFields(char *string)
{
  return string_n_split(string, fieldsAmount(string), "; ");
}

void fillPersonWithFields(Person *person, char **fields)
{
  person->region = string_duplicate(fields[0]);
  person->fullName = string_duplicate(fields[1]);
  person->age = atoi(fields[2]);
  person->phoneNumber = string_duplicate(fields[3]);
  person->id = string_duplicate(fields[4]);
  person->balance = atol(fields[5]);
}

void initPerson(Person *person)
{
  person->region = string_new();
  person->fullName = string_new();
  person->age = 0;
  person->phoneNumber = string_new();
  person->id = string_new();
  person->balance = 0;
}

char *personToString(Person person)
{
  return string_from_format("%s | %d | %s | %.30s | %s", person.region, person.age, person.id, person.fullName, person.phoneNumber);
}