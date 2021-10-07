#ifndef EMI_EXTRACTOR
#define EMI_EXTRACTOR

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct EMI_File
{
    FILE *file_pointer;
    uint8_t file_count;
    uint32_t *toc[3];
};

struct EMI_File *open_read_EMI_file(const char *filename);
uint32_t from_little_endian(uint8_t *buffer, int size);
uint8_t *read_from_EMI_file(struct EMI_File *emi_file, int index);
void extract_from_EMI_file(struct EMI_File *emi_file, int index, const char *filename);
void close_EMI_file(struct EMI_File *emi_file);

#endif
