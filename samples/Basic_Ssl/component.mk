#
ifeq ($(ENABLE_MALLOC_COUNT),1)
COMPONENT_DEPENDS	+= malloc_count
COMPONENT_CXXFLAGS	+= -DENABLE_MALLOC_COUNT=1
endif

ENABLE_SSL = 1
