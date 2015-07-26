#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
  size_t size;
  char *name;
} field;

int size_from_type(char *str) {
  if (strcmp(str, "int") == 0) {
    return sizeof(int);
  } else if (strcmp(str, "char") == 0) {
    return sizeof(char);
  } else if (strcmp(str, "short") == 0) {
    return sizeof(short);
  } else if (strcmp(str, "long") == 0) {
    return sizeof(long);
  } else if (strcmp(str, "float") == 0) {
    return sizeof(float);
  } else if (strcmp(str, "double") == 0) {
    return sizeof(double);
  }

  return -1;
}

char *substr(char *str, size_t len) {
  char *result = malloc(len + 1);
  memcpy(result, str, len);
  result[len] = '\0';
  return result;
}

bool is_identifier_char(char c) {
  return isalnum(c) || c == '_';
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

field *get_fields(char *def) {
  int num_fields = get_num_fields(def);
  field *fields = malloc(sizeof(field) * num_fields);

  char *curr = def;
  int idx = 0;
  while((curr = strstr(curr, "FIELD"))) {
    curr += 5; // strlen("FIELD")

    // advance past field type
    while (curr[0] != ',') curr++;

    while (!is_identifier_char(curr[0])) curr++;
    char *end = curr;
    while (is_identifier_char(end[0])) end++;
    size_t field_length = end - curr;

    fields[idx++].name = substr(curr, field_length);
  }
  return fields;
}

#define DEF(NAME) NAME##_def
#define FIELDS(NAME) NAME##_fields
#define NUM_FIELDS(NAME) NAME##_num_fields

#define FIELD(TYPE, NAME) \
  TYPE NAME

#define REFLECTIVE(NAME, ...) \
  typedef struct { \
    __VA_ARGS__    \
  } NAME;          \
  char *DEF(NAME) = #__VA_ARGS__; \
  field *FIELDS(NAME) = NULL;     \
  int NUM_FIELDS(NAME) = -1;      \
  field *NAME##_get_fields() {                \
    if (FIELDS(NAME) == NULL) {               \
      FIELDS(NAME) = get_fields(DEF(NAME));   \
    }                                         \
    return FIELDS(NAME);                      \
  }                                           \
  int NAME##_get_num_fields() {                       \
    if (NUM_FIELDS(NAME) < 0) {                       \
      NUM_FIELDS(NAME) = get_num_fields(DEF(NAME));   \
    }                                                 \
    return NUM_FIELDS(NAME);                          \
  }

REFLECTIVE(item,
  FIELD(int, id);
  FIELD(char *, content);
);

int main() {
  item *item = malloc(sizeof(item));
  int num_fields = item_get_num_fields();
  field *fields = item_get_fields();
  for (int i = 0; i < num_fields; ++i) {
    printf("%s\n", fields[i].name);
  }

  free(item);
  return 0;
}
