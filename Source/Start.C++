#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include "../Libraries/xdg-shell-client-protocol.h"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "./Renderers/Blend2D.hpp"
#define WIDTH  640
#define HEIGHT 480

// Global Wayland objects
struct wl_display *display;
struct wl_compositor *compositor;
struct wl_surface *surface;
struct wl_shm *shm;
struct wl_shm_pool *pool;
struct wl_buffer *buffer;
struct xdg_wm_base *xdg_wm_base;
struct xdg_surface *xdg_surface;
struct xdg_toplevel *xdg_toplevel;
int fd;

// Creates a shared memory file
int create_shm_file(size_t size) {
    int fd = shm_open("/wayland_shm", O_CREAT | O_RDWR, 0600);
    if (fd < 0) return -1;
    ftruncate(fd, size);
    return fd;
}

// Creates a Wayland buffer for rendering
void create_wayland_buffer() {
    size_t size = WIDTH * HEIGHT * 4; // 4 bytes per pixel (RGBA)

    // Create shared memory
    fd = create_shm_file(size);
    if (fd < 0) {
        perror("shm_open failed");
        return;
    }

    // Map shared memory to process space
    shm_data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_data == MAP_FAILED) {
        perror("mmap failed");
        return;
    }

    // Create Wayland shared memory pool and buffer
    pool = wl_shm_create_pool(shm, fd, size);
    buffer = wl_shm_pool_create_buffer(pool, 0, WIDTH, HEIGHT, WIDTH * 4, WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
}

// Handles Wayland registry events
void registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version) {
    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = (wl_compositor*) wl_registry_bind(registry, id, &wl_compositor_interface, 1);
    } else if (strcmp(interface, "wl_shm") == 0) {
        shm = (wl_shm*) wl_registry_bind(registry, id, &wl_shm_interface, 1);
    } else if (strcmp(interface, "xdg_wm_base") == 0) {
        xdg_wm_base = static_cast<struct xdg_wm_base*>(wl_registry_bind(registry, id, &xdg_wm_base_interface, 1));
    }
}

// Handles `xdg_wm_base` ping events (mandatory)
void xdg_wm_base_ping_handler(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct wl_registry_listener registry_listener = {
    registry_handler,
    NULL
};

static const struct xdg_wm_base_listener wm_base_listener = {
    xdg_wm_base_ping_handler
};

// Creates an XDG toplevel window
void create_window() {
    if (!compositor || !xdg_wm_base) {
        fprintf(stderr, "Compositor or xdg_wm_base missing!\n");
        return;
    }

    // Create a Wayland surface
    surface = wl_compositor_create_surface(compositor);
    if (!surface) {
        fprintf(stderr, "Failed to create surface\n");
        return;
    }

    // Create xdg_surface (shell surface)
    xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, surface);
    xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);

    // Set window title
    xdg_toplevel_set_title(xdg_toplevel, "Wayland + Blend2D");

    // Map the surface (this makes it visible)
    wl_surface_commit(surface);
}

// Main function
int main() {
    // Connect to Wayland display server
    display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland display\n");
        return -1;
    }

    // Get Wayland registry
    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display);

    // Ensure required interfaces are available
    if (!compositor || !shm || !xdg_wm_base) {
        fprintf(stderr, "Required Wayland interfaces not available\n");
        return -1;
    }

    // Add xdg_wm_base event listener
    xdg_wm_base_add_listener(xdg_wm_base, &wm_base_listener, NULL);

    // Create window and buffer
    create_window();
    create_wayland_buffer();

    Blend2D_renderTest();

    // Attach the buffer to the surface
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_damage(surface, 0, 0, WIDTH, HEIGHT);
    wl_surface_commit(surface);

    // Main event loop
    while (wl_display_dispatch(display) != -1) {}

    // Cleanup
    wl_display_disconnect(display);
    munmap(shm_data, WIDTH * HEIGHT * 4);
    close(fd);
    return 0;
}

