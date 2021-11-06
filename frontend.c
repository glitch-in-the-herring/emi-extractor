#include "emi_extractor.h"

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        printf("Usage: %s source [output prefix]\n", argv[0]);
        return 1;
    }

    char buffer[25];

    struct EMI_File *file = open_read_EMI_file(argv[1]);

    for (int i = 0; i < file->file_count; i++)
    {
        snprintf(buffer, 25, "%s_%i.bin", argv[1], i);
        extract_from_EMI_file(file, i, buffer);
    }

    close_EMI_file(file);
}
