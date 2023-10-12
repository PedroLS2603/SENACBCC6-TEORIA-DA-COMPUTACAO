#include <string.h>

char * remove_quebra(char * string) {
  for(int i = 0; i<strlen(string); i++) {
    if(string[i] == '\n') {
      string[i] = '\0';
    }
  }

  return string;
}