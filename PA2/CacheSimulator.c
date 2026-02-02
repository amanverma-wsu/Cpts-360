#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// cache line - stores valid bit and tag
typedef struct {
    int valid;        // is this line valid?
    unsigned int tag; // which block is this?
} CacheLine;

// the whole cache
typedef struct {
    CacheLine* lines; // all the lines
    int num_lines;    // how many
    int block_size;   // size of each block
} Cache;

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
int parse_address(const char* line, unsigned int* address);

int main(int argc, char* argv[]) {
    // need 3 args
    if (argc != 4) {
        printf("Usage: %s <cache_size> <block_size> <input_file>\n", argv[0]);
        return 1;
    }

    int cache_size = atoi(argv[1]);
    int block_size = atoi(argv[2]);
    char* filename = argv[3];

    // check inputs are valid
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

    int accesses = 0;
    int hits = 0;
    int misses = 0;

    char line[256];
    unsigned int address;
    
    // read each line
    while (fgets(line, sizeof(line), fp) != NULL) {
        size_t len = strlen(line);
        // if line is too long, skip rest
        if (len > 0 && line[len - 1] != '\n') {
            int ch;
            while ((ch = fgetc(fp)) != '\n' && ch != EOF);
        }
        
        if (parse_address(line, &address)) {
            accesses++;
            
            if (access_cache(cache, address)) {
                hits++;
            } else {
                misses++;
            }
        }
    }

    fclose(fp);
    print_stats(accesses, hits, misses);
    free_cache(cache);

    return 0;
}

// make a new cache
Cache* create_cache(int cache_size, int block_size) {
    Cache* cache = (Cache*)malloc(sizeof(Cache));
    if (cache == NULL) {
        return NULL;
    }

    cache->block_size = block_size;
    cache->num_lines = cache_size / block_size;

    cache->lines = (CacheLine*)malloc(sizeof(CacheLine) * cache->num_lines);
    if (cache->lines == NULL) {
        free(cache);
        return NULL;
    }

    init_cache_lines(cache);

    return cache;
}

// set all lines to empty
void init_cache_lines(Cache* cache) {
    for (int i = 0; i < cache->num_lines; i++) {
        cache->lines[i].valid = 0;
        cache->lines[i].tag = 0;
    }
}

// delete cache
void free_cache(Cache* cache) {
    if (cache != NULL) {
        if (cache->lines != NULL) {
            free(cache->lines);
        }
        free(cache);
    }
}

// check if address is in cache
int access_cache(Cache* cache, unsigned int address) {
    unsigned int block_address = calculate_block_address(cache, address);
    unsigned int index = calculate_index(cache, block_address);
    unsigned int tag = calculate_tag(cache, block_address);
    
    if (is_cache_hit(cache, index, tag)) {
        return 1;
    }
    
    update_cache_line(cache, index, tag);
    
    return 0;
}

// divide addr by block size
unsigned int calculate_block_address(Cache* cache, unsigned int address) {
    return address / cache->block_size;
}

// which line in cache?
unsigned int calculate_index(Cache* cache, unsigned int block_address) {
    return block_address % cache->num_lines;
}

// the tag part
unsigned int calculate_tag(Cache* cache, unsigned int block_address) {
    return block_address / cache->num_lines;
}

// is it in cache?
int is_cache_hit(Cache* cache, unsigned int index, unsigned int tag) {
    return (cache->lines[index].valid && cache->lines[index].tag == tag);
}

// put new data in line
void update_cache_line(Cache* cache, unsigned int index, unsigned int tag) {
    cache->lines[index].valid = 1;
    cache->lines[index].tag = tag;
}

// read address from string
int parse_address(const char* line, unsigned int* address) {
    if (line == NULL || line[0] == '\0') {
        return 0;
    }
    
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    
    if (line[0] == '\0') {
        return 0;
    }
    
    if (line[0] == '0' && line[1] != '\0' && (line[1] == 'x' || line[1] == 'X')) {  // need to check line[1] or it crashes
        if (sscanf(line, "%x", address) == 1) {
            return 1;
        }
        return 0;  // bad hex, don't try decimal
    }
    
    if (sscanf(line, "%u", address) == 1) {
        return 1;
    }
    
    return 0;
}

// print config info
void print_cache_config(int cache_size, int block_size, int num_lines) {
    printf("=== Cache Configuration ===\n");
    printf("Cache Type: Direct-mapped\n");
    printf("Cache Size: %d bytes\n", cache_size);
    printf("Block Size: %d bytes\n", block_size);
    printf("Number of Lines: %d\n", num_lines);
    printf("Address Size: 32 bits\n");
    printf("Replacement Policy: Direct-mapped (no choice)\n");
    printf("===========================\n\n");
}

// print results
void print_stats(int accesses, int hits, int misses) {
    double hit_rate = 0.0;
    
    if (accesses > 0) {
        hit_rate = (double)hits / accesses * 100.0;
    }

    printf("=== Cache Statistics ===\n");
    printf("Total memory accesses: %d\n", accesses);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);
    printf("Cache hit rate: %.2f%%\n", hit_rate);
}