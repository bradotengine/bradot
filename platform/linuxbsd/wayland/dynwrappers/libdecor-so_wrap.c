// This file is generated. Do not edit!
// see https://github.com/hpvb/dynload-wrapper for details
// generated by ./generate-wrapper.py 0.3 on 2022-12-12 10:55:19
// flags: ./generate-wrapper.py --include /usr/include/libdecor-0/libdecor.h --sys-include <libdecor-0/libdecor.h> --soname libdecor-0.so.0 --init-name libdecor --output-header libdecor-so_wrap.h --output-implementation libdecor-so_wrap.c --omit-prefix wl_
//
// EDIT: This has been handpatched to properly report the pointer type of the window_state argument of libdecor_configuration_get_window_state.
#include <stdint.h>

#define libdecor_unref libdecor_unref_dylibloader_orig_libdecor
#define libdecor_new libdecor_new_dylibloader_orig_libdecor
#define libdecor_get_fd libdecor_get_fd_dylibloader_orig_libdecor
#define libdecor_dispatch libdecor_dispatch_dylibloader_orig_libdecor
#define libdecor_decorate libdecor_decorate_dylibloader_orig_libdecor
#define libdecor_frame_ref libdecor_frame_ref_dylibloader_orig_libdecor
#define libdecor_frame_unref libdecor_frame_unref_dylibloader_orig_libdecor
#define libdecor_frame_set_visibility libdecor_frame_set_visibility_dylibloader_orig_libdecor
#define libdecor_frame_is_visible libdecor_frame_is_visible_dylibloader_orig_libdecor
#define libdecor_frame_set_parent libdecor_frame_set_parent_dylibloader_orig_libdecor
#define libdecor_frame_set_title libdecor_frame_set_title_dylibloader_orig_libdecor
#define libdecor_frame_get_title libdecor_frame_get_title_dylibloader_orig_libdecor
#define libdecor_frame_set_app_id libdecor_frame_set_app_id_dylibloader_orig_libdecor
#define libdecor_frame_set_capabilities libdecor_frame_set_capabilities_dylibloader_orig_libdecor
#define libdecor_frame_unset_capabilities libdecor_frame_unset_capabilities_dylibloader_orig_libdecor
#define libdecor_frame_has_capability libdecor_frame_has_capability_dylibloader_orig_libdecor
#define libdecor_frame_show_window_menu libdecor_frame_show_window_menu_dylibloader_orig_libdecor
#define libdecor_frame_popup_grab libdecor_frame_popup_grab_dylibloader_orig_libdecor
#define libdecor_frame_popup_ungrab libdecor_frame_popup_ungrab_dylibloader_orig_libdecor
#define libdecor_frame_translate_coordinate libdecor_frame_translate_coordinate_dylibloader_orig_libdecor
#define libdecor_frame_set_min_content_size libdecor_frame_set_min_content_size_dylibloader_orig_libdecor
#define libdecor_frame_set_max_content_size libdecor_frame_set_max_content_size_dylibloader_orig_libdecor
#define libdecor_frame_resize libdecor_frame_resize_dylibloader_orig_libdecor
#define libdecor_frame_move libdecor_frame_move_dylibloader_orig_libdecor
#define libdecor_frame_commit libdecor_frame_commit_dylibloader_orig_libdecor
#define libdecor_frame_set_minimized libdecor_frame_set_minimized_dylibloader_orig_libdecor
#define libdecor_frame_set_maximized libdecor_frame_set_maximized_dylibloader_orig_libdecor
#define libdecor_frame_unset_maximized libdecor_frame_unset_maximized_dylibloader_orig_libdecor
#define libdecor_frame_set_fullscreen libdecor_frame_set_fullscreen_dylibloader_orig_libdecor
#define libdecor_frame_unset_fullscreen libdecor_frame_unset_fullscreen_dylibloader_orig_libdecor
#define libdecor_frame_is_floating libdecor_frame_is_floating_dylibloader_orig_libdecor
#define libdecor_frame_close libdecor_frame_close_dylibloader_orig_libdecor
#define libdecor_frame_map libdecor_frame_map_dylibloader_orig_libdecor
#define libdecor_frame_get_xdg_surface libdecor_frame_get_xdg_surface_dylibloader_orig_libdecor
#define libdecor_frame_get_xdg_toplevel libdecor_frame_get_xdg_toplevel_dylibloader_orig_libdecor
#define libdecor_state_new libdecor_state_new_dylibloader_orig_libdecor
#define libdecor_state_free libdecor_state_free_dylibloader_orig_libdecor
#define libdecor_configuration_get_content_size libdecor_configuration_get_content_size_dylibloader_orig_libdecor
#define libdecor_configuration_get_window_state libdecor_configuration_get_window_state_dylibloader_orig_libdecor
#include <libdecor-0/libdecor.h>
#undef libdecor_unref
#undef libdecor_new
#undef libdecor_get_fd
#undef libdecor_dispatch
#undef libdecor_decorate
#undef libdecor_frame_ref
#undef libdecor_frame_unref
#undef libdecor_frame_set_visibility
#undef libdecor_frame_is_visible
#undef libdecor_frame_set_parent
#undef libdecor_frame_set_title
#undef libdecor_frame_get_title
#undef libdecor_frame_set_app_id
#undef libdecor_frame_set_capabilities
#undef libdecor_frame_unset_capabilities
#undef libdecor_frame_has_capability
#undef libdecor_frame_show_window_menu
#undef libdecor_frame_popup_grab
#undef libdecor_frame_popup_ungrab
#undef libdecor_frame_translate_coordinate
#undef libdecor_frame_set_min_content_size
#undef libdecor_frame_set_max_content_size
#undef libdecor_frame_resize
#undef libdecor_frame_move
#undef libdecor_frame_commit
#undef libdecor_frame_set_minimized
#undef libdecor_frame_set_maximized
#undef libdecor_frame_unset_maximized
#undef libdecor_frame_set_fullscreen
#undef libdecor_frame_unset_fullscreen
#undef libdecor_frame_is_floating
#undef libdecor_frame_close
#undef libdecor_frame_map
#undef libdecor_frame_get_xdg_surface
#undef libdecor_frame_get_xdg_toplevel
#undef libdecor_state_new
#undef libdecor_state_free
#undef libdecor_configuration_get_content_size
#undef libdecor_configuration_get_window_state
#include <dlfcn.h>
#include <stdio.h>
void (*libdecor_unref_dylibloader_wrapper_libdecor)(struct libdecor*);
struct libdecor* (*libdecor_new_dylibloader_wrapper_libdecor)(struct wl_display*,struct libdecor_interface*);
int (*libdecor_get_fd_dylibloader_wrapper_libdecor)(struct libdecor*);
int (*libdecor_dispatch_dylibloader_wrapper_libdecor)(struct libdecor*, int);
struct libdecor_frame* (*libdecor_decorate_dylibloader_wrapper_libdecor)(struct libdecor*,struct wl_surface*,struct libdecor_frame_interface*, void*);
void (*libdecor_frame_ref_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
void (*libdecor_frame_unref_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
void (*libdecor_frame_set_visibility_dylibloader_wrapper_libdecor)(struct libdecor_frame*, bool);
bool (*libdecor_frame_is_visible_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
void (*libdecor_frame_set_parent_dylibloader_wrapper_libdecor)(struct libdecor_frame*,struct libdecor_frame*);
void (*libdecor_frame_set_title_dylibloader_wrapper_libdecor)(struct libdecor_frame*,const char*);
const char* (*libdecor_frame_get_title_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
void (*libdecor_frame_set_app_id_dylibloader_wrapper_libdecor)(struct libdecor_frame*,const char*);
void (*libdecor_frame_set_capabilities_dylibloader_wrapper_libdecor)(struct libdecor_frame*,enum libdecor_capabilities);
void (*libdecor_frame_unset_capabilities_dylibloader_wrapper_libdecor)(struct libdecor_frame*,enum libdecor_capabilities);
bool (*libdecor_frame_has_capability_dylibloader_wrapper_libdecor)(struct libdecor_frame*,enum libdecor_capabilities);
void (*libdecor_frame_show_window_menu_dylibloader_wrapper_libdecor)(struct libdecor_frame*,struct wl_seat*, uint32_t, int, int);
void (*libdecor_frame_popup_grab_dylibloader_wrapper_libdecor)(struct libdecor_frame*,const char*);
void (*libdecor_frame_popup_ungrab_dylibloader_wrapper_libdecor)(struct libdecor_frame*,const char*);
void (*libdecor_frame_translate_coordinate_dylibloader_wrapper_libdecor)(struct libdecor_frame*, int, int, int*, int*);
void (*libdecor_frame_set_min_content_size_dylibloader_wrapper_libdecor)(struct libdecor_frame*, int, int);
void (*libdecor_frame_set_max_content_size_dylibloader_wrapper_libdecor)(struct libdecor_frame*, int, int);
void (*libdecor_frame_resize_dylibloader_wrapper_libdecor)(struct libdecor_frame*,struct wl_seat*, uint32_t,enum libdecor_resize_edge);
void (*libdecor_frame_move_dylibloader_wrapper_libdecor)(struct libdecor_frame*,struct wl_seat*, uint32_t);
void (*libdecor_frame_commit_dylibloader_wrapper_libdecor)(struct libdecor_frame*,struct libdecor_state*,struct libdecor_configuration*);
void (*libdecor_frame_set_minimized_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
void (*libdecor_frame_set_maximized_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
void (*libdecor_frame_unset_maximized_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
void (*libdecor_frame_set_fullscreen_dylibloader_wrapper_libdecor)(struct libdecor_frame*,struct wl_output*);
void (*libdecor_frame_unset_fullscreen_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
bool (*libdecor_frame_is_floating_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
void (*libdecor_frame_close_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
void (*libdecor_frame_map_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
struct xdg_surface* (*libdecor_frame_get_xdg_surface_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
struct xdg_toplevel* (*libdecor_frame_get_xdg_toplevel_dylibloader_wrapper_libdecor)(struct libdecor_frame*);
struct libdecor_state* (*libdecor_state_new_dylibloader_wrapper_libdecor)( int, int);
void (*libdecor_state_free_dylibloader_wrapper_libdecor)(struct libdecor_state*);
bool (*libdecor_configuration_get_content_size_dylibloader_wrapper_libdecor)(struct libdecor_configuration*,struct libdecor_frame*, int*, int*);
bool (*libdecor_configuration_get_window_state_dylibloader_wrapper_libdecor)(struct libdecor_configuration*,enum libdecor_window_state*);
int initialize_libdecor(int verbose) {
  void *handle;
  char *error;
  handle = dlopen("libdecor-0.so.0", RTLD_LAZY);
  if (!handle) {
    if (verbose) {
      fprintf(stderr, "%s\n", dlerror());
    }
    return(1);
  }
  dlerror();
// libdecor_unref
  *(void **) (&libdecor_unref_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_unref");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_new
  *(void **) (&libdecor_new_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_new");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_get_fd
  *(void **) (&libdecor_get_fd_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_get_fd");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_dispatch
  *(void **) (&libdecor_dispatch_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_dispatch");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_decorate
  *(void **) (&libdecor_decorate_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_decorate");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_ref
  *(void **) (&libdecor_frame_ref_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_ref");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_unref
  *(void **) (&libdecor_frame_unref_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_unref");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_set_visibility
  *(void **) (&libdecor_frame_set_visibility_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_set_visibility");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_is_visible
  *(void **) (&libdecor_frame_is_visible_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_is_visible");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_set_parent
  *(void **) (&libdecor_frame_set_parent_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_set_parent");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_set_title
  *(void **) (&libdecor_frame_set_title_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_set_title");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_get_title
  *(void **) (&libdecor_frame_get_title_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_get_title");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_set_app_id
  *(void **) (&libdecor_frame_set_app_id_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_set_app_id");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_set_capabilities
  *(void **) (&libdecor_frame_set_capabilities_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_set_capabilities");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_unset_capabilities
  *(void **) (&libdecor_frame_unset_capabilities_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_unset_capabilities");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_has_capability
  *(void **) (&libdecor_frame_has_capability_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_has_capability");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_show_window_menu
  *(void **) (&libdecor_frame_show_window_menu_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_show_window_menu");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_popup_grab
  *(void **) (&libdecor_frame_popup_grab_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_popup_grab");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_popup_ungrab
  *(void **) (&libdecor_frame_popup_ungrab_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_popup_ungrab");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_translate_coordinate
  *(void **) (&libdecor_frame_translate_coordinate_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_translate_coordinate");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_set_min_content_size
  *(void **) (&libdecor_frame_set_min_content_size_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_set_min_content_size");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_set_max_content_size
  *(void **) (&libdecor_frame_set_max_content_size_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_set_max_content_size");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_resize
  *(void **) (&libdecor_frame_resize_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_resize");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_move
  *(void **) (&libdecor_frame_move_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_move");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_commit
  *(void **) (&libdecor_frame_commit_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_commit");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_set_minimized
  *(void **) (&libdecor_frame_set_minimized_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_set_minimized");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_set_maximized
  *(void **) (&libdecor_frame_set_maximized_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_set_maximized");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_unset_maximized
  *(void **) (&libdecor_frame_unset_maximized_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_unset_maximized");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_set_fullscreen
  *(void **) (&libdecor_frame_set_fullscreen_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_set_fullscreen");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_unset_fullscreen
  *(void **) (&libdecor_frame_unset_fullscreen_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_unset_fullscreen");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_is_floating
  *(void **) (&libdecor_frame_is_floating_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_is_floating");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_close
  *(void **) (&libdecor_frame_close_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_close");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_map
  *(void **) (&libdecor_frame_map_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_map");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_get_xdg_surface
  *(void **) (&libdecor_frame_get_xdg_surface_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_get_xdg_surface");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_frame_get_xdg_toplevel
  *(void **) (&libdecor_frame_get_xdg_toplevel_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_frame_get_xdg_toplevel");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_state_new
  *(void **) (&libdecor_state_new_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_state_new");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_state_free
  *(void **) (&libdecor_state_free_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_state_free");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_configuration_get_content_size
  *(void **) (&libdecor_configuration_get_content_size_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_configuration_get_content_size");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
// libdecor_configuration_get_window_state
  *(void **) (&libdecor_configuration_get_window_state_dylibloader_wrapper_libdecor) = dlsym(handle, "libdecor_configuration_get_window_state");
  if (verbose) {
    error = dlerror();
    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    }
  }
return 0;
}