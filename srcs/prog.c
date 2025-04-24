#include "woody.h"

char shellcode[] = "\x57\xbf\x01\x00\x00\x00\x48\xbe\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\xba\x0c\x00\x00\x00\x0f\x05\x5f\xff\xe7";


int create_woody(ElfInfo *info) {

    //sauvgarde de l'entry point
    Elf64_Addr original_entry = info->ehdr->e_entry;

    int fd_woody = open("woody", O_CREAT | O_RDWR | O_TRUNC, 0777);
    if (fd_woody == -1)
    {
        perror("open");
        return 1;
    }

    Elf64_Ehdr *new_ehdr = malloc(sizeof(Elf64_Ehdr));
    if (new_ehdr == NULL)
    {
        perror("malloc");
        return 1;
    }
    memcpy(new_ehdr, info->ehdr, sizeof(Elf64_Ehdr));
    new_ehdr->e_entry = info->ehdr->e_shoff + info->shdr[info->ehdr->e_shnum - 1].sh_size;
 
    printf("original entry point: %lx\n", original_entry);
    printf("new entry point: %lx\n", new_ehdr->e_entry);

    write(fd_woody, new_ehdr, sizeof(Elf64_Ehdr));

    lseek(fd_woody, info->ehdr->e_phoff, SEEK_SET);
    write(fd_woody, info->phdr, sizeof(Elf64_Phdr) * info->ehdr->e_phnum);

    lseek(fd_woody, info->ehdr->e_shoff, SEEK_SET);
    write(fd_woody, info->shdr, sizeof(Elf64_Shdr) * info->ehdr->e_shnum);

    for (int i = 0; i < info->ehdr->e_shnum; i++) {
        if (info->shdr[i].sh_type != SHT_NOBITS && info->shdr[i].sh_size > 0) {
            lseek(fd_woody, info->shdr[i].sh_offset, SEEK_SET);
            write(fd_woody, info->data + info->shdr[i].sh_offset, info->shdr[i].sh_size);
        }
    }


    //ajouter le shellcode
    lseek(fd_woody, 0, SEEK_END);
    write(fd_woody, shellcode, sizeof(shellcode));

    close(fd_woody);
    return 0;
}

/*
etapes :
✔ lire le fichier 
✔ creer woody
✔ copier tout le fichier dans woody
✔ save l'entry point
ajouter le code a la fin 
modifier l'entry point pour qu'il pointe vers le code
a la fin du code de drecyptage faire un jump vers l'entry point original
*/

