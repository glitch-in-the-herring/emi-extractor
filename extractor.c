#include "emi_extractor.h"

struct EMI_File *open_read_EMI_file(const char *filename)
{
    struct EMI_File *new_EMI_file = malloc(sizeof(struct EMI_File));
    new_EMI_file->file_pointer = fopen(filename, "rb");
    uint8_t buffer[4];
    uint8_t header[8];

    fseek(new_EMI_file->file_pointer, 0x8, SEEK_SET);
    fread(header, 1, 8, new_EMI_file->file_pointer);

    uint8_t magic[8] = {0x4d, 0x41, 0x54, 0x48, 0x5f, 0x54, 0x42, 0x4c};
    for (int i = 0; i < 8; i++)
    {
        if (header[i] != magic[i])
        {
            fclose(new_EMI_file->file_pointer);
            free(new_EMI_file);
            return NULL;
        }
    }

    fseek(new_EMI_file->file_pointer, 0x0, SEEK_SET);
    fread(&(new_EMI_file->file_count), 1, 1, new_EMI_file->file_pointer);

    for (int i = 0; i < 3; i++)
        new_EMI_file->toc[i] = malloc(new_EMI_file->file_count * sizeof(uint32_t));

    uint32_t number_buffer_0;
    uint32_t number_buffer_1 = 0;
    uint32_t address = 0x800;

    for (int i = 0; i < new_EMI_file->file_count; i++)
    {
        fseek(new_EMI_file->file_pointer, 0x10 + 0x10 * i, SEEK_SET);
        new_EMI_file->toc[1][i] = address;

        fread(buffer, 1, 4, new_EMI_file->file_pointer);
        number_buffer_0 = from_little_endian(buffer, 4);
        new_EMI_file->toc[0][i] = number_buffer_0;
        number_buffer_1 = number_buffer_0;

        fseek(new_EMI_file->file_pointer, 0x10 + 0x10 * i + 8, SEEK_SET);
        fread(buffer, 1, 4, new_EMI_file->file_pointer);
        number_buffer_0 = from_little_endian(buffer, 4);
        new_EMI_file->toc[2][i] = number_buffer_0;

        if (number_buffer_1 % 0x800 != 0)
            number_buffer_1 = 0x800 * ((number_buffer_1 + 0x800) / 0x800);

        address += number_buffer_1;
    }

    return new_EMI_file;
}

uint32_t from_little_endian(uint8_t *buffer, int size)
{
    uint32_t result = 0;
    for (int i = size - 1; i >= 0; i--)
        result = buffer[i] << 8 * i | result;

    return result;    
}

uint8_t *read_from_EMI_file(struct EMI_File *emi_file, int index)
{
    uint8_t *buffer = malloc(emi_file->toc[0][index]);

    fseek(emi_file->file_pointer, emi_file->toc[1][index], SEEK_SET);
    fread(buffer, 1, emi_file->toc[0][index], emi_file->file_pointer);

    return buffer;
}

void extract_from_EMI_file(struct EMI_File *emi_file, int index, const char *filename)
{
    uint8_t *buffer = read_from_EMI_file(emi_file, index);
    FILE *file_pointer = fopen(filename, "wb");
    fwrite(buffer, 1, emi_file->toc[0][index], file_pointer);
    fclose(file_pointer);

    free(buffer);
}

void close_EMI_file(struct EMI_File *emi_file)
{
    fclose(emi_file->file_pointer);
    for (int i = 0; i < 3; i++)
        free(emi_file->toc[i]);
    free(emi_file);
}
