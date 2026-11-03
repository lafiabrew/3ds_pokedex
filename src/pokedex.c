#include "pokedex.h"

#include <stddef.h>

static const pkdx_entry_t k_dex[PKDX_COUNT] = {
    { 6,   "CHARIZARD",  1, "FIRE",     "FLYING",   "BLAZE / SOLAR POWER",     { 78,  84,  78, 109,  85, 100 }, "FIRE/DRAGON IN MANY ROM HACKS" },
    { 25,  "PIKACHU",    1, "ELECTRIC", "NONE",     "STATIC / LIGHTNING ROD",  { 35,  55,  40,  50,  50,  90 }, "LIGHT BALL DOUBLES ATK & SPA" },
    { 94,  "GENGAR",     1, "GHOST",    "POISON",   "CURSED BODY / LEVITATE",  { 60,  65,  60, 130,  75, 110 }, "LEVITATE RESTORED IN DRAYANO HACKS" },
    { 130, "GYARADOS",   1, "WATER",    "FLYING",   "INTIMIDATE / MOXIE",      { 95, 125,  79,  60, 100,  81 }, "MEGA: WATER/DARK MOLD BREAKER" },
    { 149, "DRAGONITE",  1, "DRAGON",   "FLYING",   "MULTISCALE / INNER FOCUS",{ 91, 134,  95, 100, 100,  80 }, "EXTREME SPEED + MULTISCALE SWEEPER" },
    { 248, "TYRANITAR",  2, "ROCK",     "DARK",     "SAND STREAM / UNNERVED",  {100, 134, 110,  95, 100,  61 }, "SETS SAND (+50% ROCK SPD)" },
    { 257, "BLAZIKEN",   3, "FIRE",     "FIGHTING", "BLAZE / SPEED BOOST",     { 80, 120,  70, 110,  70,  80 }, "HIDDEN ABILITY SPEED BOOST" },
    { 282, "GARDEVOIR",  3, "PSYCHIC",  "FAIRY",    "TRACE / PIXILATE (MEGA)", { 68,  65,  65, 125, 115,  80 }, "HYPER VOICE PIXILATE WALLBREAKER" },
    { 373, "SALAMENCE",  3, "DRAGON",   "FLYING",   "INTIMIDATE / AERILATE",   { 95, 135,  80, 110,  80, 100 }, "RUTILE RUBY BOSS ACE" },
    { 376, "METAGROSS",  3, "STEEL",    "PSYCHIC",  "CLEAR BODY / TOUGH CLAWS",{ 80, 135, 130,  95,  90,  70 }, "STEVEN STONE SIGNATURE PSEUDO" },
    { 384, "RAYQUAZA",   3, "DRAGON",   "FLYING",   "AIR LOCK / DELTA STREAM", {105, 150,  90, 150,  90,  95 }, "DRAGON ASCENT MEGA EVOLUTION" },
    { 392, "INFERNAPE",  4, "FIRE",     "FIGHTING", "BLAZE / IRON FIST",       { 76, 104,  71, 104,  71, 108 }, "RENEGADE PLATINUM IRON FIST BUFF" },
    { 445, "GARCHOMP",   4, "DRAGON",   "GROUND",   "SAND VEIL / ROUGH SKIN",  {108, 130,  95,  80,  85, 102 }, "CYNTHIA ACE (102 BASE SPEED)" },
    { 448, "LUCARIO",    4, "FIGHTING", "STEEL",    "JUSTIFIED / ADAPTABILITY",{ 70, 110,  70, 115,  70,  90 }, "MEGA ADAPTABILITY CLOSE COMBAT" },
    { 658, "GRENINJA",   6, "WATER",    "DARK",     "PROTEAN / BATTLE BOND",   { 72,  95,  67, 103,  71, 122 }, "ASH-GRENINJA 640 BST FORM" },
    { 778, "MIMIKYU",    7, "GHOST",    "FAIRY",    "DISGUISE",                { 55,  90,  80,  50, 105,  96 }, "PHOTONIC SUN TOTEM BOSS" }
};

int pkdx_total_entries(void) {
    return PKDX_COUNT;
}

const pkdx_entry_t *pkdx_get_by_index(int idx) {
    if (idx < 0 || idx >= PKDX_COUNT) return NULL;
    return &k_dex[idx];
}

const pkdx_entry_t *pkdx_find_by_dex_num(uint16_t dex_num) {
    for (int i = 0; i < PKDX_COUNT; i++) {
        if (k_dex[i].dex_num == dex_num) return &k_dex[i];
    }
    return NULL;
}

uint16_t pkdx_calc_bst(const pkdx_entry_t *entry) {
    if (entry == NULL) return 0;
    uint16_t sum = 0;
    for (int i = 0; i < 6; i++) {
        sum = (uint16_t)(sum + entry->base_stats[i]);
    }
    return sum;
}
