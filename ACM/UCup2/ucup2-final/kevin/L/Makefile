UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CXX = g++-14
else
	CXX = g++
endif

CXXFLAGS = -O2 -std=gnu++23 -Wall -Wextra -Wno-unused-result -pedantic -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -Wshadow

DEBUGFLAGS = -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=undefined -fno-sanitize-recover=all -fstack-protector -D_FORTIFY_SOURCE=2

DEBUG = false
ifeq ($(DEBUG),true)
	CXXFLAGS += $(DEBUGFLAGS)
endif

ifeq ($(UNAME_S),Darwin)
	CXXFLAGS += -Wl,-stack_size,0x1c000000
endif
