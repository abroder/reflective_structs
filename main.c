#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool is_identifier_char(char c) {
	return isalpha(c) || c == '_';
}

char **get_fields(char *def) {
	char *curr = def;
	while((curr = strstr(curr, "FIELD"))) {
		curr += 5; // strlen("FIELD")
		
		// advance past field type
		while (curr[0] != ',') curr++;

		while (!is_identifier_char(curr[0])) curr++;
		char *end = curr;
		while (is_identifier_char(end[0])) end++;
		size_t field_length = end - curr;

		char *field = malloc(field_length + 1);
		memcpy(field, curr, field_length);
		field[field_length] = '\0';
		printf("%s\n", field);
		free(field);		
	}
	return NULL;
}

int get_num_fields(char *def) {
  int num_fields = 0;
  int idx = 0;
  while (def[idx] != '\0') {
    if (def[idx] == ')') num_fields++;
    idx++;
  }
  return num_fields;
}

#define DEF(NAME) NAME##_def
#define FIELDS(NAME) NAME##_fields
#define NUM_FIELDS(NAME) NAME##_num_fields

#define FIELD(TYPE, NAME) \
  TYPE NAME;

#define REFLECTIVE(NAME, ...) \
  typedef struct { \
    __VA_ARGS__    \
  } NAME;          \
  char *DEF(NAME) = #__VA_ARGS__; \
  char **FIELDS(NAME) = NULL;     \
  int NUM_FIELDS(NAME) = -1;      \
  char **NAME##_get_fields() {                \
  	if (FIELDS(NAME) == NULL) {               \
  		FIELDS(NAME) = get_fields(DEF(NAME));   \
  	}									                        \
  	return FIELDS(NAME);                      \
  }                                           \
  int NAME##_get_num_fields() {                       \
    if (NUM_FIELDS(NAME) < 0) {                       \
      NUM_FIELDS(NAME) = get_num_fields(DEF(NAME));   \
    }                                                 \
    return NUM_FIELDS(NAME);                          \
  }

REFLECTIVE(item,
  FIELD(int, id)
  FIELD(char *, content)
)

int main() {
  item *item = malloc(sizeof(item));
  item_get_fields();
  printf("%d\n", item_get_num_fields());

  free(item);
  return 0;
}
