#ifndef FILE_H
#define FILE_H

#include "config.h"

size_t BootCount(const char *idxDir,const char *idxFile);
void writeLastSent(const char *idxDir,const char *idxFile,size_t NumLines);

#endif