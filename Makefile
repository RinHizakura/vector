COPS = -Wall -Wextra -Werror
GIT_HOOKS := .git/hooks/applied
BUILD_DIR = build

ifeq ("$(ORI)","1")
	COPS += -DORI
endif

ifeq ("$(NAIVE)","1")
	COPS += -DNAIVE
endif


all: $(GIT_HOOKS) exec
clean: 
	rm -rf $(BUILD_DIR) exec

diff: exec answer.txt
	./exec > out.txt
	colordiff out.txt answer.txt 

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

# '<' for current dependency, '@' for current target
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	gcc $(COPS) -g -c $< -o $@ 

C_FILES = $(wildcard *.c)
OBJ_FILES = $(C_FILES:%.c=$(BUILD_DIR)/%.o)

answer.txt:
	python ./scripts/check.py > answer.txt

exec: $(OBJ_FILES)
	gcc -o exec $(OBJ_FILES)

