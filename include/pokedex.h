#ifndef POKEDEX_H
#define POKEDEX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PKDX_COUNT 16

typedef struct {
    uint16_t dex_num;
    const char *name;
    uint8_t gen;
    const char *type1;
    const char *type2;
    const char *abilities;
    uint16_t base_stats[6]; /* HP, Atk, Def, SpA, SpD, Spe */
    const char *romhack_notes;
} pkdx_entry_t;

int pkdx_total_entries(void);
const pkdx_entry_t *pkdx_get_by_index(int idx);
const pkdx_entry_t *pkdx_find_by_dex_num(uint16_t dex_num);
uint16_t pkdx_calc_bst(const pkdx_entry_t *entry);

#endif /* POKEDEX_H */
