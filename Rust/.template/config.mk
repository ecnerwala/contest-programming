# config.mk
LANG := rust
ifeq ($(DEBUG),true)
TARGET := target/debug/$(PROBLEM_NAME)
else
TARGET := target/release/$(PROBLEM_NAME)
endif
EXECUTE := ./$(TARGET)
CLEAN_TARGETS := target
