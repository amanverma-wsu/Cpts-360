obj-m += sysprog_counter.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)
PROC := /proc/sysprog_counter

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

test: all
	@echo "=============================="
	@echo "  sysprog_counter Test Suite  "
	@echo "=============================="

	@# Load fresh module
	@sudo rmmod sysprog_counter 2>/dev/null || true
	@sudo insmod sysprog_counter.ko
	@sleep 0.3

	@# TEST 1: Initial value is 0
	@echo "[TEST 1] Initial value should be 0"
	@cat $(PROC)

	@# TEST 2: Write 42
	@echo "[TEST 2] Write 42, read back"
	@echo 42 | sudo tee $(PROC)
	@cat $(PROC)

	@# TEST 3: Write negative number
	@echo "[TEST 3] Write -7, read back"
	@echo -7 | sudo tee $(PROC)
	@cat $(PROC)

	@# TEST 4: Reset to 0
	@echo "[TEST 4] Write 0 (reset)"
	@echo 0 | sudo tee $(PROC)
	@cat $(PROC)

	@# TEST 5: Large number
	@echo "[TEST 5] Write 999999, read back"
	@echo 999999 | sudo tee $(PROC)
	@cat $(PROC)

	@# TEST 6: Invalid input (should be rejected, counter stays at 999999)
	@echo "[TEST 6] Write 'abc' (invalid, counter should stay 999999)"
	@echo abc | sudo tee $(PROC) || true
	@cat $(PROC)

	@# Unload
	@sudo rmmod sysprog_counter
	@echo ""
	@echo "Done. Check dmesg for kernel-side logs:"
	@sudo dmesg | tail -15

.PHONY: all clean test
