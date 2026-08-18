CC := gcc
NAME := main
SOURCEDIR := src
BUILDDIR := build
DEPS_DIR := deps

CPPFLAGS := -Ideps -Ideps/glad/include -Ideps/freetype/include
CFLAGS := -Wall -MMD -MP
LDFLAGS := 
LDLIBS := -lm -lglfw -lGL -lfreetype

SRCS := $(shell find $(SOURCEDIR) -name '*.c')

# Glad
SRCS += $(DEPS_DIR)/glad/glad.c

OBJS := $(patsubst %.c,$(BUILDDIR)/%.o,$(SRCS))

.PHONY: clean run help deps freetype

DEPS_BUILD_DIR := $(BUILDDIR)/deps
DEPS_INSTALL_DIR := $(DEPS_DIR)/install

all: $(NAME)

deps: freetype
	@mkdir -p $(DEPS_BUILD_DIR) $(DEPS_INSTALL_DIR)

freetype:
	cmake -S $(DEPS_DIR)/freetype -B $(DEPS_BUILD_DIR)/freetype
	@mkdir -p $(DEPS_BUILD_DIR)/freetype
	cmake --build $(DEPS_BUILD_DIR)/freetype

$(NAME): deps $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(NAME) $(LDLIBS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $(CFLAGS) $< -o $@



help: # Show this help
	@echo "Usage: make [TARGET]..."
	@echo ""
	@echo "Available targets:"
	@grep -E '^[a-zA-Z0-9_-]+:.*#' Makefile | sort | \
		awk 'BEGIN { FS = ":.*# " } { printf " - \x1b[1m%-10s\x1b[0m %s\n", $$1, $$2 }'

run: $(NAME) # Run the application
	./$(NAME)

clean: # Remove all build files
	rm -rf $(BUILDDIR)
	rm -f $(NAME)

-include $(OBJS:.o=.d)
