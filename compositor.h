#ifndef __COMPOSITOR_H
#define __COMPOSITOR_H

#include <stdbool.h>
#include <stdint.h>
#include <wayland-server-core.h>
#include <xkbcommon/xkbcommon.h>

struct compositor;
struct window;

struct compositor_button_event {
    uint32_t button;
    uint32_t time_msec;
    bool state;
};

struct compositor_key_event {
    const xkb_keysym_t *syms;
    int nsyms;
    uint32_t modifiers;
    uint32_t time_msec;
    bool state;
};

struct compositor_motion_event {
    double x, y;
    uint32_t time_msec;
};

typedef bool (*compositor_button_func_t)(struct compositor_button_event event);
typedef bool (*compositor_key_func_t)(struct compositor_key_event event);
typedef void (*compositor_motion_func_t)(struct compositor_motion_event event);

struct compositor_vtable {
    compositor_button_func_t button;
    compositor_key_func_t key;
    compositor_motion_func_t motion;
};

struct synthetic_input {};

// Attempts to create a new compositor. Returns NULL on failure.
struct compositor *compositor_create(struct compositor_vtable vtable);

// Releases resources associated with `compositor`.
void compositor_destroy(struct compositor *compositor);

// Returns the internal event loop of `compositor`, to which various callbacks can be added.
struct wl_event_loop *compositor_get_loop(struct compositor *compositor);

// Runs the compositor event loop. Returns true on success, false on failure.
bool compositor_run(struct compositor *compositor);

// Configures the given window to the given position and size.
void compositor_configure_window(struct window *window, int16_t x, int16_t y, int16_t w, int16_t h);

// Transfers input focus to the given window. If `window` is NULL, input focus is removed from
// whichever window currently has it (if any).
void compositor_focus_window(struct compositor *compositor, struct window *window);

// Returns the number of existing windows. If there are more than 0 windows, a buffer is allocated
// and placed in the user-provided `windows` pointer. The caller must free the buffer.
int compositor_get_windows(struct compositor *compositor, struct window ***windows);

// Attempts to get the process ID of the given `window`. Returns -1 on failure.
pid_t compositor_get_window_pid(struct window *window);

// Sends a keyboard input to the given `window`.
void compositor_send_key(struct window *window, struct synthetic_input key);

#endif
