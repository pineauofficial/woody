#include "woody.h"

int main(int argc, char **argv) {
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <elf_file>\n", argv[0]);
        return 1;
    }

    ElfInfo info = {0};

    if (init_elf_info(argv[1], &info) != 0)
        return 1;

    if (info.ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    {
        fprintf(stderr, "File architecture not supported. x86_64 only\n");
        cleanup_elf_info(&info);
        return 1;
    }
    
    //clef tmp
    for (int i = 0; i < 16; i++)
        info.key[i] = rand() % 256;
    
    printf("key_value: ");
    for (int i = 0; i < 16; i++)
        printf("%02X", info.key[i]);
    printf("\n");
    
    // encrypt_text_section(&info);
    
    if (create_woody(&info) != 0)
    {
        cleanup_elf_info(&info);
        return 1;
    }
    
    cleanup_elf_info(&info);
    return 0;
}
