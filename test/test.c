#include <stdio.h>

void never_call(void) {
    printf("[*] wow how did you manage to call this?0000\n");
    return;
}

int main(void) {
    printf("[*] you ran this binary!\n");
    return (0);
}

//readelf -h test.elf | grep "Entry" -> Entry point address: 0x1060
//objdump -D test.elf | grep "never_call" -> 0000000000001149 <never_call>:
//objcopy --set-section-flags .text=alloc,code --set-start=0x1149 test.elf test_modified.elf