#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commons/log.h"
#include "commons/string.h"
#include "commons/txt.h"
#include "commons/collections/list.h"
typedef struct Person
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
char *personToString(Person *);
void writePeopleInFile(t_list *);
bool orderByRegion(void *, void *);
bool orderByAge(void *, void *);
bool orderByRegionAndAge(void *, void *);
t_log *initLogger();
void terminateProgram(FILE *, t_log *, t_list *);

int main()
{

  FILE *inputFile = fopen("Personas.txt", "r");

  t_log *logger = initLogger();

  t_list *people = list_create();

  while (!feof(inputFile))
  {
    char *line = getLine(inputFile);
    char **personFields = getFields(line);

    Person *person = malloc(sizeof(Person *));
    initPerson(person);
    fillPersonWithFields(person, personFields);

    if (person->age >= 18)
    {
      list_add(people, person);
      if (person->balance < 100)
        log_info(logger, (const char *)personToString(person));
    }
    else
      continue;
  }

  list_sort(people, &orderByRegionAndAge);
  writePeopleInFile(people);

  terminateProgram(inputFile, logger, people);

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

char *personToString(Person *person)
{
  return string_from_format("%s | %d | %s | %.30s | %s\n", person->region, person->age, person->id, person->fullName, person->phoneNumber);
}

void writePeopleInFile(t_list *people)
{
  FILE *outputFile = txt_open_for_append("Salida.txt");

  for (int i = 0; i < list_size(people); i++)
  {
    Person *person = (Person *)list_get(people, i);
    txt_write_in_file(outputFile, personToString(person));
  }

  txt_close_file(outputFile);
}

bool orderByRegion(void *person, void *otherPerson)
{
  return strcmp(((Person *)person)->region, ((Person *)otherPerson)->region) < 0;
}

bool orderByAge(void *person, void *otherPerson)
{
  return ((Person *)person)->age < ((Person *)otherPerson)->age;
}

bool orderByRegionAndAge(void *person, void *otherPerson)
{
  bool regionBool = orderByRegion(person, otherPerson);

  if (!regionBool)
    return orderByAge(person, otherPerson);
  else
    return regionBool;
}

t_log *initLogger()
{
  return log_create("Personas.log", "Persona", true, LOG_LEVEL_INFO);
}

void terminateProgram(FILE *file, t_log *logger, t_list *list)
{
  log_destroy(logger);
  list_destroy(list);
  txt_close_file(file);
}