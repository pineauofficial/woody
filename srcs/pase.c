#include "woody.h"

int init_elf_info(const char *filename, ElfInfo *info) {

    info->fd = open(filename, O_RDONLY);
    if (info->fd == -1)
    {
        perror("open");
        return 1;
    }

    info->size = lseek(info->fd, 0, SEEK_END);
    if ((int)info->size == -1)
    {
        perror("lseek");
        close(info->fd);
        return 1;
    }
    
    if (lseek(info->fd, 0, SEEK_SET) == -1)
    {
        perror("lseek");
        close(info->fd);
        return 1;
    }

    info->data = mmap(NULL, info->size, PROT_READ, MAP_PRIVATE, info->fd, 0);
    if (info->data == MAP_FAILED)
    {
        perror("mmap");
        close(info->fd);
        return 1;
    }

    info->ehdr = (Elf64_Ehdr *)info->data;
    info->phdr = (Elf64_Phdr *)(info->data + info->ehdr->e_phoff);
    info->shdr = (Elf64_Shdr *)(info->data + info->ehdr->e_shoff);
    info->shstrtab = &info->shdr[info->ehdr->e_shstrndx];
    info->shstrtab_p = info->data + info->shstrtab->sh_offset;
    
    // trouver la sectioon .text
    for (int i = 0; i < info->ehdr->e_shnum; i++)
    {
        char *section_name = info->shstrtab_p + info->shdr[i].sh_name;
        if (strcmp(section_name, ".text") == 0)
        {
            info->text_section = &info->shdr[i];
            break;
        }
    }
    
    if (info->text_section == NULL)
    {
        fprintf(stderr, "Failed to find .text section\n");
        munmap(info->data, info->size);
        close(info->fd);
        return 1;
    }
    
    return 0;
}

void cleanup_elf_info(ElfInfo *info) {
    if (info->data)
        munmap(info->data, info->size);
    if (info->fd != -1)
        close(info->fd);
}