NAME = woody_woodpacker
SRCS_DIR = srcs/
ASM_DIR = asm/
SRCS = $(wildcard $(SRCS_DIR)/*.c)
ASM_SRCS = $(wildcard $(ASM_DIR)/*.s)
OBJS = $(SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.s=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -I./includes
ASM = nasm
ASMFLAGS = -f elf64 -i $(ASM_DIR)
LDFLAGS = -no-pie

all: $(NAME)

$(NAME): $(OBJS) $(ASM_OBJS)
	$(CC) $(ASM_OBJS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(SRCS_DIR)%.o: $(SRCS_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(ASM_DIR)%.o: $(ASM_DIR)%.s
	@echo "Assembling $< to $@"
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(ASM_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re