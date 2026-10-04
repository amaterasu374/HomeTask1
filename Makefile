CSOURCES := $(shell find . -name "*.c")
CFLAGS   := -Wall -Wextra -Wfloat-equal -O2 -MMD -Wstrict-prototypes

debug: CFLAGS    := $(CFLAGS) -g
debug: LDFLAGS   :=

all: CFLAGS      := $(CFLAGS) -g -fsanitize=address -fsanitize=leak \
                    -fsanitize=undefined -fsanitize=null \
                    -fsanitize=bounds-strict -fstack-protector-all
all: LDFLAGS     := -fsanitize=address -fsanitize=leak \
                    -fsanitize=undefined -fsanitize=null \
                    -fsanitize=bounds-strict -fstack-protector-all

coverage: CFLAGS := $(CFLAGS) -g -fsanitize=address -fsanitize=leak \
                    -fsanitize=undefined -fsanitize=null \
                    -fsanitize=bounds-strict -fstack-protector-all \
                    -fprofile-arcs -ftest-coverage
coverage: LDFLAGS := -fsanitize=address -fsanitize=leak \
                     -fsanitize=undefined -fsanitize=null \
                     -fsanitize=bounds-strict -fstack-protector-all \
                     -fprofile-arcs -ftest-coverage

werror: CFLAGS   := $(CFLAGS) -Werror -g -fsanitize=address -fsanitize=leak \
                    -fsanitize=undefined -fsanitize=null \
                    -fsanitize=bounds-strict -fstack-protector-all
werror: LDFLAGS  := -fsanitize=address -fsanitize=leak \
                    -fsanitize=undefined -fsanitize=null \
                    -fsanitize=bounds-strict -fstack-protector-all

LDLIBS := -lm

all:      main
debug:    main
coverage: main
werror:   main

OBJECTS := $(CSOURCES:%.c=%.o)

main: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

DEPS := $(shell find . -name "*.d")
-include $(DEPS)

clean:
	rm -f main
	rm -f *.o
	rm -f *.d
	rm -f *.gcda *.gcno *.gcov

.PHONY: all debug coverage werror clean
