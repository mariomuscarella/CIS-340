// Description: Prints strings in ascending or descending order

#include <stdio.h>
#include <string.h>

/*  Printout constant array of characters in a specified order
 *  @param sarray An array of constant strings
 *  @param flag The flag to determine order of printout
 */
void custout(const char* sarray[], const char* flag ) {
  int i, j;
  const char *ascend_flags[] = {"-f", "-F"};
  const char *descend_flags[] = {"-b", "-B"};
  int ascend_flag = 0, descend_flag = 0;
  
  // determine if the specified flag is valid
  if (flag != NULL) {
    for (i = 0; i < 2; i++) {
      if (strcmp(flag, ascend_flags[i]) == 0) {
        ascend_flag = 1;
        break; 
      }
      else if (strcmp(flag, descend_flags[i]) == 0) {
        descend_flag = 1;
        break;
      }
    }
  }

  // only sort array if a valid flag is specified
  if (ascend_flag || descend_flag) {
    for (i = 0; sarray[i] != NULL; i++) {
      for (j = i + 1; sarray[j] != NULL; j++) {
        if (ascend_flag) {
          if (strcmp(sarray[i], sarray[j]) > 0) {
            const char *temp = sarray[i];
            sarray[i] = sarray[j];
            sarray[j] = temp; 
          }
        }
	else {
	  if (strcmp(sarray[i], sarray[j]) < 0) {
            const char *temp = sarray[i];
            sarray[i] = sarray[j];
            sarray[j] = temp;
          } 
        }
      }
    }
  }
  
  // print string regardless of order
  for (i = 0; sarray[i] != NULL; i++) {
    printf("%s\n", sarray[i]);
  }
}
