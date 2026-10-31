CXX = g++
CXXFLAGS = -O2 -std=gnu++17 -Wall -Wextra -Wno-unused-result -pedantic -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -Wshadow

DEBUGFLAGS = -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fstack-protector -D_FORTIFY_SOURCE=2

DEBUG = false
ifeq ($(DEBUG),true)
	CXXFLAGS += $(DEBUGFLAGS)
endif