COMPONENT_SRCDIRS := \
	src \
	src/Arch/$(SMING_ARCH)

COMPONENT_INCDIRS := $(COMPONENT_SRCDIRS)

COMPONENT_DOXYGEN_INPUT := src

#
COMPONENT_RELINK_VARS += SPISOFT_DELAY_FIXED
SPISOFT_DELAY_FIXED ?= 0
COMPONENT_CPPFLAGS := -DSPISOFT_DELAY_FIXED=$(SPISOFT_DELAY_FIXED)

#
COMPONENT_RELINK_VARS += SPISOFT_DELAY_VARIABLE
SPISOFT_DELAY_VARIABLE ?= 0
ifeq ($(SPISOFT_DELAY_VARIABLE),1)
COMPONENT_CPPFLAGS := -DSPISOFT_DELAY_VARIABLE=1
endif

#
COMPONENT_VARS += ENABLE_SPI_DEBUG
ENABLE_SPI_DEBUG ?= 0
ifeq ($(ENABLE_SPI_DEBUG),1)
GLOBAL_CFLAGS += -DSPI_DEBUG=1
endif
