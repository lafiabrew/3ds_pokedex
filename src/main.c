#include "pk_platform.h"
#include "pokedex.h"

#include <stdio.h>
#include <string.h>

typedef struct {
    char sd_root[512];
    int selected;
    int scroll;
    char status_msg[128];
} pkdx_app_t;

static void sync_scroll(pkdx_app_t *app) {
    int vis = 8;
    int tot = pkdx_total_entries();
    if (app->selected < app->scroll) app->scroll = app->selected;
    else if (app->selected >= app->scroll + vis) app->scroll = app->selected - vis + 1;
    if (app->scroll > tot - vis) app->scroll = tot - vis;
    if (app->scroll < 0) app->scroll = 0;
}

static void app_init(pkdx_app_t *app, const char *sd_root) {
    memset(app, 0, sizeof(*app));
    snprintf(app->sd_root, sizeof(app->sd_root), "%s",
             sd_root != NULL ? sd_root : pk_platform_default_sd_root());
    snprintf(app->status_msg, sizeof(app->status_msg),
             "POKEDEX 3DS READY: %d COMPETITIVE & ROMHACK ENTRIES", pkdx_total_entries());
    pk_log_event(app->sd_root, "pokedex", "INFO", app->status_msg);
}

static void app_step(pkdx_app_t *app, const pk_input_t *in) {
    int tot = pkdx_total_entries();
    if (in->down & PK_BTN_DUP)   app->selected = (app->selected + tot - 1) % tot;
    if (in->down & PK_BTN_DDOWN) app->selected = (app->selected + 1) % tot;
    if (in->down & PK_BTN_DLEFT) {
        app->selected -= 4;
        if (app->selected < 0) app->selected = 0;
    }
    if (in->down & PK_BTN_DRIGHT) {
        app->selected += 4;
        if (app->selected >= tot) app->selected = tot - 1;
    }
    if (in->touch_just_pressed && in->touch_y >= 28 && in->touch_y < 28 + 8 * 22) {
        int row = (in->touch_y - 28) / 22;
        int idx = app->scroll + row;
        if (idx >= 0 && idx < tot) app->selected = idx;
    }
    sync_scroll(app);
    const pkdx_entry_t *e = pkdx_get_by_index(app->selected);
    if (e != NULL) {
        snprintf(app->status_msg, sizeof(app->status_msg),
                 "#%03u %s (GEN %u) | BST: %u | %s/%s",
                 (unsigned)e->dex_num, e->name, (unsigned)e->gen,
                 (unsigned)pkdx_calc_bst(e), e->type1, e->type2);
    }
}

static void app_render(const pkdx_app_t *app) {
    pk_render_begin_frame();
    pk_render_begin_target(PK_TARGET_TOP_LEFT, PK_RGBA8(22, 14, 18, 255));
    pk_draw_rect(0, 0, 400, 26, PK_RGBA8(165, 32, 46, 255));
    pk_draw_text(8, 7, 0.48f, PK_RGBA8(255, 240, 195, 255),
                 "POKEDEX 3DS - GEN 1-7 & ROMHACK STAT ENCYCLOPEDIA");

    const pkdx_entry_t *e = pkdx_get_by_index(app->selected);
    if (e != NULL) {
        char line[160];
        snprintf(line, sizeof(line), "#%03u %s  [GEN %u]   TYPES: %s / %s   BST: %u",
                 (unsigned)e->dex_num, e->name, (unsigned)e->gen,
                 e->type1, e->type2, (unsigned)pkdx_calc_bst(e));
        pk_draw_text(10, 34, 0.46f, PK_RGBA8(255, 225, 95, 255), line);

        snprintf(line, sizeof(line), "ABILITIES: %s", e->abilities);
        pk_draw_text(10, 52, 0.44f, PK_RGBA8(175, 235, 255, 255), line);

        static const char *const k_stat_names[6] = { "HP ", "ATK", "DEF", "SPA", "SPD", "SPE" };
        for (int s = 0; s < 6; s++) {
            float y = 74.0f + (float)s * 18.0f;
            uint16_t v = e->base_stats[s];
            snprintf(line, sizeof(line), "%s %3u", k_stat_names[s], (unsigned)v);
            pk_draw_text(14.0f, y, 0.44f, PK_RGBA8(235, 240, 250, 255), line);
            float bar_w = (float)v * 1.6f;
            if (bar_w > 260.0f) bar_w = 260.0f;
            uint32_t bcol = v >= 110 ? PK_RGBA8(85, 245, 145, 255)
                                     : (v >= 80 ? PK_RGBA8(255, 215, 80, 255)
                                                : PK_RGBA8(255, 110, 95, 255));
            pk_draw_rect(82.0f, y - 1.0f, bar_w, 12.0f, bcol);
        }
        snprintf(line, sizeof(line), "HACK NOTE: %s", e->romhack_notes);
        pk_draw_text(10, 190, 0.43f, PK_RGBA8(255, 195, 140, 255), line);
    }

    pk_draw_rect(0, 216, 400, 24, PK_RGBA8(48, 20, 28, 255));
    pk_draw_text(8, 222, 0.44f, PK_RGBA8(130, 250, 175, 255), app->status_msg);

    /* Bottom Screen: Scrollable Dex List + Scrollbar */
    pk_render_begin_target(PK_TARGET_BOTTOM, PK_RGBA8(16, 12, 16, 255));
    pk_draw_rect(0, 0, 320, 24, PK_RGBA8(120, 26, 38, 255));
    pk_draw_text(8, 6, 0.44f, PK_RGBA8(255, 255, 255, 255), "NATIONAL & ROMHACK DEX (TOUCH / D-PAD)");
    for (int row = 0; row < 8; row++) {
        int idx = app->scroll + row;
        const pkdx_entry_t *it = pkdx_get_by_index(idx);
        if (it == NULL) break;
        float y = 28.0f + (float)row * 22.0f;
        bool sel = (idx == app->selected);
        pk_draw_rect(6.0f, y, 298.0f, 19.0f,
                     sel ? PK_RGBA8(155, 42, 56, 255) : PK_RGBA8(28, 22, 28, 255));
        char rline[96];
        snprintf(rline, sizeof(rline), "#%03u %-12s [%s/%s] BST:%u",
                 (unsigned)it->dex_num, it->name, it->type1, it->type2, (unsigned)pkdx_calc_bst(it));
        pk_draw_text(12.0f, y + 5.0f, 0.42f, PK_RGBA8(245, 240, 245, 255), rline);
    }
    pk_draw_scrollbar(310.0f, 28.0f, 174.0f, pkdx_total_entries(), 8, app->scroll,
                      PK_RGBA8(36, 24, 30, 255), PK_RGBA8(255, 110, 120, 255));
    pk_draw_text(8, 218, 0.42f, PK_RGBA8(200, 185, 195, 255),
                 "U/D:SCROLL | L/R:PAGE | START:EXIT");
    pk_render_end_frame();
}

int main(int argc, char **argv) {
    const char *sd_root = NULL;
    bool one_shot = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--sd-root") == 0 && i + 1 < argc) sd_root = argv[++i];
        else if (strcmp(argv[i], "--once") == 0) one_shot = true;
    }
    if (!pk_platform_init()) return 1;
    pkdx_app_t app;
    app_init(&app, sd_root);
    while (pk_platform_main_loop()) {
        pk_input_t in;
        pk_platform_poll_input(&in);
        if (in.down & PK_BTN_START) break;
        app_step(&app, &in);
        app_render(&app);
        if (one_shot) break;
    }
    pk_platform_shutdown();
    return 0;
}
