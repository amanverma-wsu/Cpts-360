#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int valid;
    unsigned int tag;
} CacheLine;

typedef struct {
    CacheLine* lines;
    int num_lines;
    int block_size;
} Cache;

// Function prototypes
Cache* create_cache(int cache_size, int block_size);
void init_cache_lines(Cache* cache);
void free_cache(Cache* cache);
int access_cache(Cache* cache, unsigned int address);
unsigned int calculate_block_address(Cache* cache, unsigned int address);
unsigned int calculate_index(Cache* cache, unsigned int block_address);
unsigned int calculate_tag(Cache* cache, unsigned int block_address);
int is_cache_hit(Cache* cache, unsigned int index, unsigned int tag);
void update_cache_line(Cache* cache, unsigned int index, unsigned int tag);
void print_stats(int accesses, int hits, int misses);
void print_cache_config(int cache_size, int block_size, int num_lines);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <cache_size> <block_size> <input_file>\n", argv[0]);
        return 1;
    }

    int cache_size = atoi(argv[1]);
    int block_size = atoi(argv[2]);
    char* filename = argv[3];

    // Basic validation
    if (cache_size <= 0 || block_size <= 0) {
        printf("Error: Cache size and block size must be positive integers\n");
        return 1;
    }

    if (cache_size % block_size != 0) {
        printf("Error: Cache size must be a multiple of block size\n");
        return 1;
    }

    Cache* cache = create_cache(cache_size, block_size);
    if (cache == NULL) {
        printf("Error: Failed to create cache\n");
        return 1;
    }

    print_cache_config(cache_size, block_size, cache->num_lines);

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Cannot open input file '%s'\n", filename);
        free_cache(cache);
        return 1;
    }

    int accesses = 0, hits = 0, misses = 0;
    unsigned int address;

    // Fix: Starter code used %x which parsed decimal as hex (e.g., "12" -> 18)
    // Using %i handles 0x prefix for hex or standard decimal automatically
    while (fscanf(fp, "%i", (int*)&address) == 1) {
        accesses++;
        if (access_cache(cache, address))
            hits++;
        else
            misses++;
    }

    fclose(fp);
    print_stats(accesses, hits, misses);
    free_cache(cache);

    return 0;
}

Cache* create_cache(int cache_size, int block_size) {
    Cache* cache = malloc(sizeof(Cache));
    if (!cache) return NULL;

    cache->block_size = block_size;
    cache->num_lines = cache_size / block_size;
    cache->lines = malloc(sizeof(CacheLine) * cache->num_lines);

    if (!cache->lines) {
        free(cache);
        return NULL;
    }

    init_cache_lines(cache);
    return cache;
}

// Ensure all lines start as "empty" (invalid)
void init_cache_lines(Cache* cache) {
    for (int i = 0; i < cache->num_lines; i++) {
        cache->lines[i].valid = 0;
        cache->lines[i].tag = 0;
    }
}

void free_cache(Cache* cache) {
    if (cache) {
        free(cache->lines);
        free(cache);
    }
}

// Logic: Check for hit first; if it's a miss, replace the line
int access_cache(Cache* cache, unsigned int address) {
    unsigned int block_address = calculate_block_address(cache, address);
    unsigned int index = calculate_index(cache, block_address);
    unsigned int tag = calculate_tag(cache, block_address);

    if (is_cache_hit(cache, index, tag))
        return 1;

    // Miss - update the line with new tag
    update_cache_line(cache, index, tag);
    return 0;
}

/* --- Address calculation helpers --- */

unsigned int calculate_block_address(Cache* cache, unsigned int address) {
    return address / cache->block_size;
}

// Map the block to a specific cache slot (modulo)
unsigned int calculate_index(Cache* cache, unsigned int block_address) {
    return block_address % cache->num_lines;
}

// Remaining bits used to verify if we have the correct block
unsigned int calculate_tag(Cache* cache, unsigned int block_address) {
    return block_address / cache->num_lines;
}

int is_cache_hit(Cache* cache, unsigned int index, unsigned int tag) {
    return cache->lines[index].valid && cache->lines[index].tag == tag;
}

void update_cache_line(Cache* cache, unsigned int index, unsigned int tag) {
    cache->lines[index].valid = 1;
    cache->lines[index].tag = tag;
}

void print_cache_config(int cache_size, int block_size, int num_lines) {
    printf("=== Cache Configuration ===\n");
    printf("Cache Type: Direct-mapped\n");
    printf("Cache Size: %d bytes\n", cache_size);
    printf("Block Size: %d bytes\n", block_size);
    printf("Number of Lines: %d\n", num_lines);
    printf("===========================\n\n");
}

void print_stats(int accesses, int hits, int misses) {
    double hit_rate = accesses ? (double)hits / accesses * 100.0 : 0.0;

    printf("=== Cache Statistics ===\n");
    printf("Total memory accesses: %d\n", accesses);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);
    printf("Cache hit rate: %.2f%%\n", hit_rate);
}