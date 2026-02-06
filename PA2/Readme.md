# Cache Memory Management Simulation

A direct-mapped cache simulator that processes memory access sequences and reports performance statistics.

## Specifications

| Parameter | Value |
|-----------|-------|
| Cache Type | Direct-mapped |
| Cache Size | Configurable (default: 64 bytes) |
| Block Size | Configurable (default: 16 bytes) |
| Address Size | 32 bits |
| Replacement Policy | Direct-mapped (deterministic) |

## Compilation

```bash
gcc -Wall -o cache_simulator cache_simulator.c
```

## Usage

```bash
./cache_simulator <cache_size> <block_size> <input_file>
```
# Cache Memory Management Simulation

A direct-mapped cache simulator that processes memory access sequences and reports performance statistics.

## Specifications

| Parameter | Value |
|-----------|-------|
| Cache Type | Direct-mapped |
| Cache Size | Configurable (default: 64 bytes) |
| Block Size | Configurable (default: 16 bytes) |
| Address Size | 32 bits |
| Replacement Policy | Direct-mapped (deterministic) |

## Compilation

```bash
gcc -Wall -o cache_simulator cache_simulator.c
```

## Usage

```bash
./cache_simulator <cache_size> <block_size> <input_file>
```

## Input Format

One memory address per line. Supports:
- Decimal: `100`, `32`, `256`
- Hexadecimal: `0x10`, `0x0C`, `0xFF`

## Output

```
=== Cache Configuration ===
Cache Type: Direct-mapped
Cache Size: 64 bytes
Block Size: 16 bytes
Number of Lines: 4
Address Size: 32 bits
Replacement Policy: Direct-mapped (no choice)
===========================

=== Cache Statistics ===
Total memory accesses: 10
Cache hits: 7
Cache misses: 3
Cache hit rate: 70.00%
```

## Test Results

| Input File | Accesses | Hits | Misses | Hit Rate |
|------------|----------|------|--------|----------|
| input_sequential.txt | 10 | 7 | 3 | 70.00% |
| input_repeated.txt | 10 | 8 | 2 | 80.00% |
| input_conflicts.txt | 8 | 0 | 8 | 0.00% |
| input_mixed.txt | 14 | 5 | 9 | 35.71% |
| input_hex.txt | 8 | 6 | 2 | 75.00% |

## Address Decomposition

```
Block Address = Address / Block Size
Index = Block Address % Number of Lines
Tag = Block Address / Number of Lines
```

## Assumptions

- Memory addresses are 32-bit unsigned integers
- Cache size must be a multiple of block size
- Cache starts empty (all valid bits = 0)
- Input uses `%i` format (auto-detects decimal/hex/octal)
## Input Format

One memory address per line. Supports:
- Decimal: `100`, `32`, `256`
- Hexadecimal: `0x10`, `0x0C`, `0xFF`

## Output

```
=== Cache Configuration ===
Cache Type: Direct-mapped
Cache Size: 64 bytes
Block Size: 16 bytes
Number of Lines: 4
Address Size: 32 bits
Replacement Policy: Direct-mapped (no choice)
===========================

=== Cache Statistics ===
Total memory accesses: 10
Cache hits: 7
Cache misses: 3
Cache hit rate: 70.00%
```

## Test Results

| Input File | Accesses | Hits | Misses | Hit Rate |
|------------|----------|------|--------|----------|
| input_sequential.txt | 10 | 7 | 3 | 70.00% |
| input_repeated.txt | 10 | 8 | 2 | 80.00% |
| input_conflicts.txt | 8 | 0 | 8 | 0.00% |
| input_mixed.txt | 14 | 5 | 9 | 35.71% |
| input_hex.txt | 8 | 6 | 2 | 75.00% |

## Address Decomposition

```
Block Address = Address / Block Size
Index = Block Address % Number of Lines
Tag = Block Address / Number of Lines
```

## Assumptions

- Memory addresses are 32-bit unsigned integers
- Cache size must be a multiple of block size
- Cache starts empty (all valid bits = 0)
- Input uses `%i` format (auto-detects decimal/hex/octal)