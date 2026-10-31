# local.mk
ifeq ($(DEBUG),true)
CARGO_FLAGS :=
else
CARGO_FLAGS := --release
endif

$(TARGET): $(PROBLEM_NAME).rs
	cargo build $(CARGO_FLAGS)
