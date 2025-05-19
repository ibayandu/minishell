CC = cc

CFLAGS = -Wall -Werror -Wextra -g
CFLAGS += -I./

VPATH = srcs

SRCS = utils.c collector.c

OBJS = $(SRCS:%.c=build/%.o)

TARGET = build/bin/collector.a

$(OBJS_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o  $@ $(IFLAGS)

$(TARGET): $(OBJS)
	ar rcs $@ $^

build/%.o: %.c
	@mkdir -p build/bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf build

.PHONY: clean
