// Description: Reads input strings from a file into a null character separated array

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "custout.h"

/*  Parse null separated strings from a specified file arg
 *  @param argc Count of command line arguments
 *  @param argv Command line arguments delimited by space
 *  @return The exit value if the operation was successful
 */
int main(int argc, char *argv[]) {
  int fd, i, j, k;
  int string_count;
  off_t char_count, byte_count;
  const char *flag = argv[2];
  char *farray; // unparsed strings
  const char **sarray; // parsed strings
  char curr, next;
 
  fd = open(argv[1], O_RDONLY); 

  if (fd < 0) {
    perror("Error opening file: ");
    return (-1);
  }

  // determine number of bytes by seeking to the end
  byte_count = lseek(fd, (off_t) 0, SEEK_END);
  lseek(fd, (off_t) 0, SEEK_SET); // reset pointer
  farray = (char*) malloc(byte_count);

  char_count = read(fd, &next, sizeof(char));
  string_count = 0;
  
  // populate farray with characters from the file
  for (i = char_count, j = 0; i < byte_count; i++, j++) {
    curr = next;
    char_count += read(fd, &next, sizeof(char));

    if (curr == '\\' && next == '0') {
      read(fd, &next, sizeof(char));
      curr = '\0';
      i++;
      farray[j] = curr;
      string_count++;
    } 
    else if (curr == '\n') {
      j--;
      char_count--; // do not count newline character
    }
    else {
      farray[j] = curr;
    }
  }

  if (curr == '\0') {
    char_count--;
  }
  else {
    string_count++;
  }
 
  sarray = (const char**) malloc(sizeof(const char*) * (string_count + 1));

  // populate sarray with strings from farray
  for (i = 0, j = 0, k = 0; i < char_count; i++) {
    if (farray[i] == '\0') {
      const char *str = &farray[k];
      sarray[j] = str;
      j++;
      k = i + 1; // set k to index of next string
    }
  }
  
  sarray[string_count] = NULL;

  custout(sarray, flag);
  free(farray);
  free(sarray);
}
