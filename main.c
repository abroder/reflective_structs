#include <stdlib.h>

#define FIELD(TYPE, NAME) \
  TYPE NAME

#define REFLECTIVE(NAME, ...) \
  typedef struct { \
    __VA_ARGS__    \
  } NAME

REFLECTIVE(item,
  FIELD(int, id);
  FIELD(char *, content);
);

int main() {
  item *item = malloc(sizeof(item));
  
  free(item);
  return 0;
}
