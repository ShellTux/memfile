TARGETS = \
					$(TARGET_DIR)/memfile

DEPS := src/memfile.c
$(TARGET_DIR)/memfile: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -o $@ $^ $(LDFLAGS)
