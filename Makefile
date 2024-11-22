PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BINDIR?=$(EXEC_PREFIX)/bin
INSTALL?=install
INSTALL_PROGRAM?=$(INSTALL)
SRC_DIR=src
BUILD_DIR=build
TEST_DIR=test
CC=clang
CFLAGS=-Wall -Wextra -Wpedantic #-Werror
LDFLAGS=-lm

ARGS?=

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Ensure build directory exists before building targets
$(BUILD_DIR)/mylight-debug: | $(BUILD_DIR)
$(BUILD_DIR)/mylight: | $(BUILD_DIR)

# Build the debug binary
$(BUILD_DIR)/mylight-debug: $(SRC_DIR)/mylight.c 
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/mylight.c -o $(BUILD_DIR)/mylight-debug

# Build the production binary
$(BUILD_DIR)/mylight: $(SRC_DIR)/mylight.c
	$(CC) $(CFLAGS) -O3 $(LDFLAGS) $(SRC_DIR)/mylight.c -o $(BUILD_DIR)/mylight

# Run the debug binary
run: $(BUILD_DIR)/mylight-debug
	./$(BUILD_DIR)/mylight-debug $(ARGS)

# Install the program
.PHONY: install
install: $(BUILD_DIR)/mylight
	$(INSTALL) -d $(DESTDIR)$(BINDIR)
	$(INSTALL_PROGRAM) $(BUILD_DIR)/mylight $(DESTDIR)$(BINDIR)/mylight

# Uninstall the program
.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/mylight

# Clean built files
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Show installation paths
.PHONY: print-paths
print-paths:
	@echo "Installation directories:"
	@echo "PREFIX      = $(PREFIX)"
	@echo "EXEC_PREFIX = $(EXEC_PREFIX)"
	@echo "BINDIR      = $(BINDIR)"
