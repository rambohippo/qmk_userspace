#ifndef RAMBOHIPPO_FUNCTIONS
#define RAMBOHIPPO_FUNCTIONS
#include "quantum.h"
#include "os_detection.h"

bool is_alt_tab_active = false;
bool is_ctrl_tab_active = false;

enum planck_layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _NUMPAD,
    _MEDIA,
    _ADJUST
};

enum planck_keycodes {
    QWERTY = SAFE_RANGE,
    ALT_TAB,
    CTAB_FW,
    CTAB_BK,
    EXDTVAL,
    WORK_FW,
    WORK_BK,
    WORK_NW,
    WORK_CL,
    WORK_SH
};

#define NUMPAD MO(_NUMPAD)
#define MEDIA MO(_MEDIA)
#define LN_DKFW LGUI(LCTL(KC_DOWN))     // Pop OS - Next Workspace
#define LN_DKBK LGUI(LCTL(KC_UP))       // Pop OS - Previous Workspace
#define LN_WNFW LGUI(LSFT(KC_DOWN))     // Pop OS - Move window to next workspace
#define LN_WNBK LGUI(LSFT(KC_UP))       // Pop OS - Move window to previous workspace
#define WN_DKFW LCTL(LGUI(KC_RGHT))     // Windows - Next Desktop
#define WN_DKBK LCTL(LGUI(KC_LEFT))     // Windows - Previous Desktop
#define WN_DKNW LCTL(LGUI(KC_D))        // Windows - New Desktop
#define WN_DKCL LCTL(LGUI(KC_F4))       // Windows - Close Desktop
#define GUI_TAB LGUI(KC_TAB)            // Gui-Tab
#define LN_WORK LGUI(KC_D)              // Pop OS - Show workspaces
#define WN_WORK LGUI(KC_TAB)            // Windows - Show workspaces
#define KC_TERM LGUI(KC_T)              // Super-T - Opens Terminal
#define BROWSER LGUI(KC_3)              // Gui-3 - Windows - Opens browser
#define FILES LGUI(KC_E)                // Gui-E - Opens file browser
#define IDEA_DN LSFT(LALT(KC_DOWN))     // Intellij Idea - Move Line Down
#define IDEA_UP LSFT(LALT(KC_UP))       // Intellij Idea - Move Line Up

void ctrl_tab_register(bool *is_ctrl_registered) {
    if (*is_ctrl_registered) return;

    register_code(KC_LEFT_CTRL);
    *is_ctrl_registered = true;
}

void ctrl_tab_unregister(bool *is_ctrl_registered) {
    if (!*is_ctrl_registered) return;

    unregister_code(KC_LEFT_CTRL);
    *is_ctrl_registered = false;
}

void alt_tab_register(bool *is_alt_registered) {
    if (*is_alt_registered) return;

    register_code(KC_LEFT_ALT);
    *is_alt_registered = true;
}

void alt_tab_unregister(bool *is_alt_registered) {
    if (!*is_alt_registered) return;

    unregister_code(KC_LEFT_ALT);
    *is_alt_registered = false;
}

void next_workspace(void) {
    switch (detected_host_os()) {
        case OS_LINUX:
            tap_code16(LN_DKFW);
            break;
        case OS_WINDOWS:
        default:
            tap_code16(WN_DKFW);
            break;
    }
}

void previous_workspace(void) {
    switch (detected_host_os()) {
        case OS_LINUX:
            tap_code16(LN_DKBK);
            break;
        case OS_WINDOWS:
        default:
            tap_code16(WN_DKBK);
            break;
    }
}

void create_workspace(void) {
    switch (detected_host_os()) {
        case OS_LINUX:
            tap_code16(LN_DKFW);
            break;
        case OS_WINDOWS:
        default:
            tap_code16(WN_DKNW);
            break;
    }
}

void remove_workspace(void) {
    switch (detected_host_os()) {
        case OS_LINUX:
            tap_code16(LN_DKBK);
            break;
        case OS_WINDOWS:
        default:
            tap_code16(WN_DKCL);
            break;
    }
}

void show_workspaces(void) {
    switch (detected_host_os()) {
        case OS_LINUX:
            tap_code16(LN_WORK);
            break;
        case OS_WINDOWS:
        default:
            tap_code16(WN_WORK);
            break;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                print("mode just switched to qwerty and this is a huge string\n");
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
        case TL_LOWR:
            // On key release, release keys for alt-tab + ctrl-tab
            if (!record->event.pressed) {
                alt_tab_unregister(&is_alt_tab_active);
                ctrl_tab_unregister(&is_ctrl_tab_active);
            }
            return true;
        case ALT_TAB:
            if (record->event.pressed) {
                alt_tab_register(&is_alt_tab_active);
                ctrl_tab_unregister(&is_ctrl_tab_active);
                tap_code(KC_TAB);
            }
            return false;
        case CTAB_FW:
            if (record->event.pressed) {
                ctrl_tab_register(&is_ctrl_tab_active);
                alt_tab_unregister(&is_alt_tab_active);
                tap_code(KC_TAB);
            }
            return false;
        case CTAB_BK:
            if (record->event.pressed) {
                ctrl_tab_register(&is_ctrl_tab_active);
                alt_tab_unregister(&is_alt_tab_active);
                tap_code16(LSFT(KC_TAB));
            }
            return false;
        case EXDTVAL:
            if (record->event.pressed) {
                tap_code(KC_LEFT_ALT);
                tap_code(KC_A);
                tap_code(KC_V);
                tap_code(KC_V);
            }
            return false;
        case WORK_FW:
            if (record->event.pressed) {
                next_workspace();
            }
            return false;
        case WORK_BK:
            if (record->event.pressed) {
                previous_workspace();
            }
            return false;
        case WORK_NW:
            if (record->event.pressed) {
                create_workspace();
            }
            return false;
        case WORK_CL:
            if (record->event.pressed) {
                remove_workspace();
            }
            return false;
        case WORK_SH:
            if (record->event.pressed) {
                show_workspaces();
            }
            return false;
    }
    return true;
}

float melody[8][2][2] = {
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}}, 
    {{440.0f, 8}, {440.0f, 24}},
};

#define JUST_MINOR_THIRD 1.2
#define JUST_MAJOR_THIRD 1.25
#define JUST_PERFECT_FOURTH 1.33333333
#define JUST_TRITONE 1.42222222
#define JUST_PERFECT_FIFTH 1.33333333

#define ET12_MINOR_SECOND 1.059463
#define ET12_MAJOR_SECOND 1.122462
#define ET12_MINOR_THIRD 1.189207
#define ET12_MAJOR_THIRD 1.259921
#define ET12_PERFECT_FOURTH 1.33484
#define ET12_TRITONE 1.414214
#define ET12_PERFECT_FIFTH 1.498307	

deferred_token tokens[8];

uint32_t reset_note(uint32_t trigger_time, void *note) {
    *(float*)note = 440.0f;
    return 0;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    cancel_deferred_exec(tokens[index]);
    if (clockwise) {
        melody[index][1][0] = melody[index][1][0] * ET12_MINOR_SECOND;
        melody[index][0][0] = melody[index][1][0] / ET12_PERFECT_FIFTH;
        audio_play_melody(&melody[index], 2, false);
    } else {
        melody[index][1][0] = melody[index][1][0] / ET12_MINOR_SECOND;
        melody[index][0][0] = melody[index][1][0] * ET12_TRITONE;
        audio_play_melody(&melody[index], 2, false);
    }
    tokens[index] = defer_exec(1000, reset_note, &melody[index][1][0]);
    return false;
}

#endif