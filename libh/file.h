#ifndef _FILE_H_
#define _FILE_H_

typedef struct String_ String;

int write_image(String *fnm, int width, int height, int *values);

int file_len(String *fnm);

String * read_file(String *fnm);

#endif
