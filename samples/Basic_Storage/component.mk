DISABLE_NETWORK := 1

# Use our custom hardware configuration
ifeq ($(SMING_ARCH),Esp32)
HWCONFIG := basic_storage
else
HWCONFIG := basic_storage-2m
endif

HOST_NETWORK_OPTIONS := --nonet

# Use direct assignment like this if within project file as Storage component not yet parsed
# Within a Component, use +=
HWCONFIG_BUILDSPECS := $(PROJECT_DIR)/build.jsonc

# Target invoked via partition table
.PHONY: user-build
user-build:
	@echo "This is a user build: $(call HwExpr,('X=%s, Y=%s' % (part.build['parameter1'], part.build['parameter2'])))"
	@echo "Filename: $(PARTITION_$(PART)_FILENAME)"
	@echo "$(call HwExpr,json_save(part.build, '$(PARTITION_$(PART)_FILENAME)'))"
