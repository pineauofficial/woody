#ifndef WOODY_H
# define WOODY_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <elf.h>
#include <ctype.h>

typedef struct {
    void *data;
    size_t size;       
    Elf64_Ehdr *ehdr;
    Elf64_Phdr *phdr;
    Elf64_Shdr *shdr;
    Elf64_Shdr *text_section;
    Elf64_Shdr *shstrtab;
    char *shstrtab_p;
    int fd;
    unsigned char key[16];
} ElfInfo;


int create_woody(ElfInfo *info);
void encrypt_text_section(ElfInfo *info);
int init_elf_info(const char *filename, ElfInfo *info);
void cleanup_elf_info(ElfInfo *info);


#endif