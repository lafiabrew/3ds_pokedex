/*
 * Unit & pass/fail tests for 3ds_pokedex:
 * - Pass-the-pass: Dex lookup by index and National Dex number, BST calculation (Garchomp = 600, Rayquaza = 680).
 * - Fail-the-fail: out-of-bounds index and nonexistent National Dex ID return NULL / 0.
 */

#include "pokedex.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    assert(pkdx_total_entries() == PKDX_COUNT);
    const pkdx_entry_t *garchomp = pkdx_find_by_dex_num(445);
    assert(garchomp != NULL);
    assert(strcmp(garchomp->name, "GARCHOMP") == 0);
    assert(pkdx_calc_bst(garchomp) == 600u);

    const pkdx_entry_t *ray = pkdx_find_by_dex_num(384);
    assert(ray != NULL && pkdx_calc_bst(ray) == 680u);

    /* Fail-the-fail negative assertions */
    assert(pkdx_get_by_index(-1) == NULL);
    assert(pkdx_get_by_index(999) == NULL);
    assert(pkdx_find_by_dex_num(9999) == NULL);
    assert(pkdx_calc_bst(NULL) == 0u);

    printf("PASS: test_pokedex all National Dex lookup, BST sum, and pass/fail checks succeeded.\n");
    return 0;
}
