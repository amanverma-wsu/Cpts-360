# PA4 — sysprog_counter Kernel Module

## Overview

A Linux kernel module that creates a `/proc/sysprog_counter` entry, exposing a
kernel-maintained integer counter to user space. User programs can read the
current value or write a new one using standard shell tools.

---

## Files

| File | Description |
|---|---|
| `sysprog_counter.c` | Kernel module source |
| `Makefile` | Builds the `.ko` against the running kernel |

---

## Prerequisites

- A Linux system with kernel headers installed:
  ```bash
  # Debian/Ubuntu
  sudo apt install linux-headers-$(uname -r)

  # Fedora/RHEL
  sudo dnf install kernel-devel
  ```
- `make`, `gcc`, and standard build tools

---

## How to Compile

```bash
make
```

This invokes the kernel build system (`kbuild`) and produces `sysprog_counter.ko`.

---

## How to Load

```bash
sudo insmod sysprog_counter.ko
```

Verify the module is loaded and the `/proc` entry exists:

```bash
lsmod | grep sysprog_counter
ls -l /proc/sysprog_counter
```

Kernel log confirmation:

```
[ ... ] sysprog_counter: module loaded — /proc/sysprog_counter created
```

---

## Usage

### Read the counter

```bash
cat /proc/sysprog_counter
# Counter value: 0
```

### Write a new value

```bash
echo 42 | sudo tee /proc/sysprog_counter
cat /proc/sysprog_counter
# Counter value: 42
```

### Additional examples

```bash
echo 0   | sudo tee /proc/sysprog_counter   # reset to zero
echo -7  | sudo tee /proc/sysprog_counter   # negative values supported
echo 999 | sudo tee /proc/sysprog_counter
cat /proc/sysprog_counter
# Counter value: 999
```

---

## How to Unload

```bash
sudo rmmod sysprog_counter
```

Kernel log confirmation:

```
[ ... ] sysprog_counter: module unloaded — /proc/sysprog_counter removed
```

Clean build artifacts:

```bash
make clean
```

---

## Implementation Notes

### `proc_read`
- Uses `snprintf` to format the counter into a kernel-side buffer.
- Uses `copy_to_user` to safely transfer bytes to user space.
- Tracks `*pos` to return `0` (EOF) on subsequent reads, which is required
  for tools like `cat` to terminate correctly.

### `proc_write`
- Rejects inputs `>= BUFFER_SIZE` (128 bytes) to prevent buffer overflows.
- Uses `copy_from_user` to safely receive the user-supplied string.
- Null-terminates the buffer before any string operations.
- Uses `kstrtoint` (kernel-safe integer parser) with `strstrip` to handle
  trailing newlines injected by `echo`.
- Logs invalid input to the kernel ring buffer via `printk(KERN_WARNING ...)`.

### Assumptions
- The module is loaded on a kernel that supports `proc_ops` (Linux ≥ 5.6).
  Older kernels use `file_operations` instead — swap `proc_ops` for
  `file_operations` and replace `.proc_read`/`.proc_write` with
  `.read`/`.write` if needed.
- Counter state is not persistent across module unload/reload (resets to 0).
- Concurrent write access is not mutex-guarded; for production use, add a
  `mutex` or `spinlock` around counter reads/writes.

---

## Checking Kernel Logs

```bash
sudo dmesg | tail -20
```
