#ifndef NK_INTERNAL_H
#define NK_INTERNAL_H

// #define NK_BUFFER_DEFAULT_INITIAL_SIZE (4*1024)
// #define NK_ASSERT assert
// #define NK_MEMSET memset
// #define NK_MEMCPY memcpy
// #define NK_SIN sin
// #define NK_COS cos
// #define NK_DTOA dtoa
// #define NK_VSNPRINTF vsnprintf

// #define NK_SQRT sqrt

#ifndef NK_POOL_DEFAULT_CAPACITY
#define NK_POOL_DEFAULT_CAPACITY 16
#endif

#ifndef NK_DEFAULT_COMMAND_BUFFER_SIZE
#define NK_DEFAULT_COMMAND_BUFFER_SIZE (4 * 1024)
#endif

#ifndef NK_BUFFER_DEFAULT_INITIAL_SIZE
#define NK_BUFFER_DEFAULT_INITIAL_SIZE (4 * 1024)
#endif

/* standard library headers */
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
#include <stdlib.h> /* malloc, free */
#endif
#ifdef NK_INCLUDE_STANDARD_IO
#include <stdio.h> /* fopen, fclose,... */
#endif
#ifdef NK_INCLUDE_STANDARD_VARARGS
#include <stdarg.h> /* valist, va_start, va_end, ... */
#endif
#ifndef NK_ASSERT
#include <assert.h>
#define NK_ASSERT(expr) assert(expr)
#endif

#define NK_DEFAULT (-1)

#ifndef NK_VSNPRINTF
/* If your compiler does support `vsnprintf` I would highly recommend
 * defining this to vsnprintf instead since `vsprintf` is basically
 * unbelievable unsafe and should *NEVER* be used. But I have to support
 * it since C89 only provides this unsafe version. */
#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || \
    (defined(__cplusplus) && (__cplusplus >= 201103L)) || \
    (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L)) || \
    (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)) || \
    defined(_ISOC99_SOURCE) || defined(_BSD_SOURCE)
#define NK_VSNPRINTF(s, n, f, a) vsnprintf(s, n, f, a)
#else
#define NK_VSNPRINTF(s, n, f, a) vsprintf(s, f, a)
#endif
#endif

#define NK_SCHAR_MIN (-127)
#define NK_SCHAR_MAX 127
#define NK_UCHAR_MIN 0
#define NK_UCHAR_MAX 256
#define NK_SSHORT_MIN (-32767)
#define NK_SSHORT_MAX 32767
#define NK_USHORT_MIN 0
#define NK_USHORT_MAX 65535
#define NK_SINT_MIN (-2147483647)
#define NK_SINT_MAX 2147483647
#define NK_UINT_MIN 0
#define NK_UINT_MAX 4294967295u

/* Make sure correct type size:
 * This will fire with a negative subscript error if the type sizes
 * are set incorrectly by the compiler, and compile out if not */
NK_STATIC_ASSERT(sizeof(nk_size) >= sizeof(void*));
NK_STATIC_ASSERT(sizeof(nk_ptr) == sizeof(void*));
NK_STATIC_ASSERT(sizeof(nk_flags) >= 4);
NK_STATIC_ASSERT(sizeof(nk_rune) >= 4);
NK_STATIC_ASSERT(sizeof(nk_ushort) == 2);
NK_STATIC_ASSERT(sizeof(nk_short) == 2);
NK_STATIC_ASSERT(sizeof(nk_uint) == 4);
NK_STATIC_ASSERT(sizeof(nk_int) == 4);
NK_STATIC_ASSERT(sizeof(nk_byte) == 1);
#ifdef NK_INCLUDE_STANDARD_BOOL
NK_STATIC_ASSERT(sizeof(nk_bool) == sizeof(bool));
#else
NK_STATIC_ASSERT(sizeof(nk_bool) == 4);
#endif

NK_GLOBAL const nk_rect nk_null_rect = {-8192.0f, -8192.0f, 16384, 16384};
#define NK_FLOAT_PRECISION 0.00000000000001

NK_GLOBAL const nk_color nk_red = {255, 0, 0, 255};
NK_GLOBAL const nk_color nk_green = {0, 255, 0, 255};
NK_GLOBAL const nk_color nk_blue = {0, 0, 255, 255};
NK_GLOBAL const nk_color nk_white = {255, 255, 255, 255};
NK_GLOBAL const nk_color nk_black = {0, 0, 0, 255};
NK_GLOBAL const nk_color nk_yellow = {255, 255, 0, 255};

/* widget */
#define nk_widget_state_reset(s) \
  if ((*(s)) & NK_WIDGET_STATE_MODIFIED) \
    (*(s)) = NK_WIDGET_STATE_INACTIVE | NK_WIDGET_STATE_MODIFIED; \
  else \
    (*(s)) = NK_WIDGET_STATE_INACTIVE;

/* math */
NK_LIB float nk_inv_sqrt(float n);
#ifndef NK_SIN
#define NK_SIN nk_sin
#define NK_SIN_IMPLEMENTATION
NK_LIB float nk_sin(float x);
#endif
#ifndef NK_COS
#define NK_COS nk_cos
#define NK_COS_IMPLEMENTATION
NK_LIB float nk_cos(float x);
#endif
NK_LIB nk_uint nk_round_up_pow2(nk_uint v);
NK_LIB nk_rect nk_shrink_make_rect(nk_rect r, float amount);
NK_LIB nk_rect nk_pad_rect(nk_rect r, nk_vec2 pad);
NK_LIB void nk_unify(nk_rect* clip, const nk_rect* a, float x0, float y0, float x1, float y1);
NK_LIB double nk_pow(double x, int n);
NK_LIB int nk_ifloord(double x);
NK_LIB int nk_ifloorf(float x);
NK_LIB int nk_iceilf(float x);
NK_LIB int nk_log10(double n);

/* util */
enum {
  NK_DO_NOT_STOP_ON_NEW_LINE,
  NK_STOP_ON_NEW_LINE,
};
NK_LIB nk_bool nk_is_lower(int c);
NK_LIB nk_bool nk_is_upper(int c);
NK_LIB int nk_to_upper(int c);
NK_LIB int nk_to_lower(int c);

#ifndef NK_MEMCPY
#define NK_MEMCPY nk_memcopy
#define NK_MEMCPY_IMPLEMENTATION
NK_LIB void* nk_memcopy(void* dst, const void* src, nk_size n);
#endif
#ifndef NK_MEMSET
#define NK_MEMSET nk_memset
#define NK_MEMSET_IMPLEMENTATION
NK_LIB void nk_memset(void* ptr, int c0, nk_size size);
#endif
NK_LIB void nk_zero(void* ptr, nk_size size);
NK_LIB char* nk_itoa(char* s, long n);
NK_LIB int nk_string_float_limit(char* string, int prec);
#ifndef NK_DTOA
#define NK_DTOA nk_dtoa
#define NK_DTOA_IMPLEMENTATION
NK_LIB char* nk_dtoa(char* s, double n);
#endif
NK_LIB int nk_text_clamp(const nk_user_font* font, const char* text, int text_len, float space, int* glyphs, float* text_width, nk_rune* sep_list, int sep_count);
NK_LIB nk_vec2 nk_text_calculate_text_bounds(const nk_user_font* font, const char* begin, int byte_len, float row_height, const char** remaining, nk_vec2* out_offset, int* glyphs, int op);
#ifdef NK_INCLUDE_STANDARD_VARARGS
NK_LIB int nk_strfmt(char* buf, int buf_size, const char* fmt, va_list args);
#endif
#ifdef NK_INCLUDE_STANDARD_IO
NK_LIB char* nk_file_load(const char* path, nk_size* siz, nk_allocator* alloc);
#endif

/* buffer */
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_LIB void* nk_malloc(nk_handle unused, void* old, nk_size size);
NK_LIB void nk_mfree(nk_handle unused, void* ptr);
#endif
NK_LIB void* nk_buffer_align(void* unaligned, nk_size align, nk_size* alignment, nk_buffer_allocation_type type);
NK_LIB void* nk_buffer_alloc(nk_buffer* b, nk_buffer_allocation_type type, nk_size size, nk_size align);
NK_LIB void* nk_buffer_realloc(nk_buffer* b, nk_size capacity, nk_size* size);

/* draw */
NK_LIB void nk_command_buffer_init(nk_command_buffer* cb, nk_buffer* b, nk_command_clipping clip);
NK_LIB void nk_command_buffer_reset(nk_command_buffer* b);
NK_LIB void* nk_command_buffer_push(nk_command_buffer* b, nk_command_type t, nk_size size);
NK_LIB void nk_draw_symbol(nk_command_buffer* out, nk_symbol_type type, nk_rect content, nk_color background, nk_color foreground, float border_width, const nk_user_font* font);

/* buffering */
NK_LIB void nk_start_buffer(nk_context* ctx, nk_command_buffer* b);
NK_LIB void nk_start(nk_context* ctx, nk_window* win);
NK_LIB void nk_start_popup(nk_context* ctx, nk_window* win);
NK_LIB void nk_finish_popup(nk_context* ctx, nk_window*);
NK_LIB void nk_finish_buffer(nk_context* ctx, nk_command_buffer* b);
NK_LIB void nk_finish(nk_context* ctx, nk_window* w);
NK_LIB void nk_build(nk_context* ctx);

/* text editor */
NK_LIB void nk_textedit_clear_state(nk_text_edit* state, nk_text_edit_type type, nk_plugin_filter filter);
NK_LIB void nk_textedit_click(nk_text_edit* state, float x, float y, const nk_user_font* font, float row_height);
NK_LIB void nk_textedit_drag(nk_text_edit* state, float x, float y, const nk_user_font* font, float row_height);
NK_LIB void nk_textedit_key(nk_text_edit* state, nk_keys key, int shift_mod, const nk_user_font* font, float row_height);

/* window */
typedef enum {
  NK_INSERT_BACK, /* inserts window into the back of list (front of screen) */
  NK_INSERT_FRONT /* inserts window into the front of list (back of screen) */
} nk_window_insert_location;
NK_LIB void* nk_create_window(nk_context* ctx);
NK_LIB void nk_remove_window(nk_context*, nk_window*);
NK_LIB void nk_free_window(nk_context* ctx, nk_window* win);
NK_LIB nk_window* nk_find_window(nk_context* ctx, nk_hash hash, const char* name);
NK_LIB void nk_insert_window(nk_context* ctx, nk_window* win, nk_window_insert_location loc);

/* pool */
NK_LIB void nk_pool_init(nk_pool* pool, nk_allocator* alloc, unsigned int capacity);
NK_LIB void nk_pool_free(nk_pool* pool);
NK_LIB void nk_pool_init_fixed(nk_pool* pool, void* memory, nk_size size);
NK_LIB nk_page_element* nk_pool_alloc(nk_pool* pool);

/* page-element */
NK_LIB nk_page_element* nk_create_page_element(nk_context* ctx);
NK_LIB void nk_link_page_element_into_freelist(nk_context* ctx, nk_page_element* elem);
NK_LIB void nk_free_page_element(nk_context* ctx, nk_page_element* elem);

/* table */
NK_LIB nk_table* nk_create_table(nk_context* ctx);
NK_LIB void nk_remove_table(nk_window* win, nk_table* tbl);
NK_LIB void nk_free_table(nk_context* ctx, nk_table* tbl);
NK_LIB void nk_push_table(nk_window* win, nk_table* tbl);
NK_LIB nk_uint* nk_add_value(nk_context* ctx, nk_window* win, nk_hash name, nk_uint value);
NK_LIB nk_uint* nk_find_value(nk_window* win, nk_hash name);

/* panel */
NK_LIB void* nk_create_panel(nk_context* ctx);
NK_LIB void nk_free_panel(nk_context*, nk_panel* pan);
NK_LIB nk_bool nk_panel_has_header(nk_flags flags, const char* title);
NK_LIB nk_vec2 nk_panel_get_padding(const nk_style* style, nk_panel_type type);
NK_LIB float nk_panel_get_border(const nk_style* style, nk_flags flags, nk_panel_type type);
NK_LIB nk_color nk_panel_get_border_color(const nk_style* style, nk_panel_type type);
NK_LIB nk_bool nk_panel_is_sub(nk_panel_type type);
NK_LIB nk_bool nk_panel_is_nonblock(nk_panel_type type);
NK_LIB nk_bool nk_panel_begin(nk_context* ctx, const char* title, nk_panel_type panel_type);
NK_LIB void nk_panel_end(nk_context* ctx);

/* layout */
NK_LIB float nk_layout_row_calculate_usable_space(const nk_style* style, nk_panel_type type, float total_space, int columns);
NK_LIB void nk_panel_layout(const nk_context* ctx, nk_window* win, float height, int cols);
NK_LIB void nk_make_row_layout(nk_context* ctx, nk_layout_format fmt, float height, int cols, int width);
NK_LIB void nk_panel_alloc_row(const nk_context* ctx, nk_window* win);
NK_LIB void nk_layout_widget_space(nk_rect* bounds, const nk_context* ctx, nk_window* win, int modify);
NK_LIB void nk_panel_alloc_space(nk_rect* bounds, const nk_context* ctx);
NK_LIB void nk_layout_peek(nk_rect* bounds, nk_context* ctx);

/* popup */
NK_LIB nk_bool nk_nonblock_begin(nk_context* ctx, nk_flags flags, nk_rect body, nk_rect header, nk_panel_type panel_type);

/* text */
typedef struct {
  nk_vec2 padding;
  nk_color background;
  nk_color text;
} nk_text;
NK_LIB void nk_widget_text(nk_command_buffer* o, nk_rect b, const char* string, int len, const nk_text* t, nk_flags a, const nk_user_font* f);
NK_LIB void nk_widget_text_wrap(nk_command_buffer* o, nk_rect b, const char* string, int len, const nk_text* t, const nk_user_font* f);

/* button */
NK_LIB nk_bool nk_do_button_behavior(nk_flags* state, nk_rect r, const nk_input* i, nk_button_behavior behavior);
NK_LIB const nk_style_item* nk_draw_button(nk_command_buffer* out, const nk_rect* bounds, nk_flags state, const nk_style_button* style);
NK_LIB nk_bool nk_do_button(nk_flags* state, nk_command_buffer* out, nk_rect r, const nk_style_button* style, const nk_input* in, nk_button_behavior behavior, nk_rect* content);
NK_LIB void nk_draw_button_text(nk_command_buffer* out, const nk_rect* bounds, const nk_rect* content, nk_flags state, const nk_style_button* style, const char* txt, int len, nk_flags text_alignment, const nk_user_font* font);
NK_LIB nk_bool nk_do_button_text(nk_flags* state, nk_command_buffer* out, nk_rect bounds, const char* string, int len, nk_flags align, nk_button_behavior behavior, const nk_style_button* style, const nk_input* in, const nk_user_font* font);
NK_LIB void nk_draw_button_symbol(nk_command_buffer* out, const nk_rect* bounds, const nk_rect* content, nk_flags state, const nk_style_button* style, nk_symbol_type type, const nk_user_font* font);
NK_LIB nk_bool nk_do_button_symbol(nk_flags* state, nk_command_buffer* out, nk_rect bounds, nk_symbol_type symbol, nk_button_behavior behavior, const nk_style_button* style, const nk_input* in, const nk_user_font* font);
NK_LIB void nk_draw_button_image(nk_command_buffer* out, const nk_rect* bounds, const nk_rect* content, nk_flags state, const nk_style_button* style, const nk_image* img);
NK_LIB nk_bool nk_do_button_image(nk_flags* state, nk_command_buffer* out, nk_rect bounds, nk_image img, nk_button_behavior b, const nk_style_button* style, const nk_input* in);
NK_LIB void nk_draw_button_text_symbol(nk_command_buffer* out, const nk_rect* bounds, const nk_rect* label, const nk_rect* symbol, nk_flags state, const nk_style_button* style, const char* str, int len, nk_symbol_type type, const nk_user_font* font);
NK_LIB nk_bool nk_do_button_text_symbol(nk_flags* state, nk_command_buffer* out, nk_rect bounds, nk_symbol_type symbol, const char* str, int len, nk_flags align, nk_button_behavior behavior, const nk_style_button* style, const nk_user_font* font, const nk_input* in);
NK_LIB void nk_draw_button_text_image(nk_command_buffer* out, const nk_rect* bounds, const nk_rect* label, const nk_rect* image, nk_flags state, const nk_style_button* style, const char* str, int len, const nk_user_font* font, const nk_image* img);
NK_LIB nk_bool nk_do_button_text_image(nk_flags* state, nk_command_buffer* out, nk_rect bounds, nk_image img, const char* str, int len, nk_flags align, nk_button_behavior behavior, const nk_style_button* style, const nk_user_font* font, const nk_input* in);

/* toggle */
typedef enum {
  NK_TOGGLE_CHECK,
  NK_TOGGLE_OPTION
} nk_toggle_type;
NK_LIB nk_bool nk_toggle_behavior(const nk_input* in, nk_rect select, nk_flags* state, nk_bool active);
NK_LIB void nk_draw_checkbox(nk_command_buffer* out, nk_flags state, const nk_style_toggle* style, nk_bool active, const nk_rect* label, const nk_rect* selector, const nk_rect* cursors, const char* string, int len, const nk_user_font* font);
NK_LIB void nk_draw_option(nk_command_buffer* out, nk_flags state, const nk_style_toggle* style, nk_bool active, const nk_rect* label, const nk_rect* selector, const nk_rect* cursors, const char* string, int len, const nk_user_font* font);
NK_LIB nk_bool nk_do_toggle(nk_flags* state, nk_command_buffer* out, nk_rect r, nk_bool* active, const char* str, int len, nk_toggle_type type, const nk_style_toggle* style, const nk_input* in, const nk_user_font* font);

/* progress */
NK_LIB nk_size nk_progress_behavior(nk_flags* state, nk_input* in, nk_rect r, nk_rect cursor, nk_size max, nk_size value, nk_bool modifiable);
NK_LIB void nk_draw_progress(nk_command_buffer* out, nk_flags state, const nk_style_progress* style, const nk_rect* bounds, const nk_rect* scursor, nk_size value, nk_size max);
NK_LIB nk_size nk_do_progress(nk_flags* state, nk_command_buffer* out, nk_rect bounds, nk_size value, nk_size max, nk_bool modifiable, const nk_style_progress* style, nk_input* in);

/* slider */
NK_LIB float nk_slider_behavior(nk_flags* state, nk_rect* logical_cursor, nk_rect* visual_cursor, nk_input* in, nk_rect bounds, float slider_min, float slider_max, float slider_value, float slider_step, float slider_steps);
NK_LIB void nk_draw_slider(nk_command_buffer* out, nk_flags state, const nk_style_slider* style, const nk_rect* bounds, const nk_rect* visual_cursor, float min, float value, float max);
NK_LIB float nk_do_slider(nk_flags* state, nk_command_buffer* out, nk_rect bounds, float min, float val, float max, float step, const nk_style_slider* style, nk_input* in, const nk_user_font* font);

/* scrollbar */
NK_LIB float nk_scrollbar_behavior(nk_flags* state, nk_input* in, int has_scrolling, const nk_rect* scroll, const nk_rect* cursor, const nk_rect* empty0, const nk_rect* empty1, float scroll_offset, float target, float scroll_step, nk_orientation o);
NK_LIB void nk_draw_scrollbar(nk_command_buffer* out, nk_flags state, const nk_style_scrollbar* style, const nk_rect* bounds, const nk_rect* scroll);
NK_LIB float nk_do_scrollbarv(nk_flags* state, nk_command_buffer* out, nk_rect scroll, int has_scrolling, float offset, float target, float step, float button_pixel_inc, const nk_style_scrollbar* style, nk_input* in, const nk_user_font* font);
NK_LIB float nk_do_scrollbarh(nk_flags* state, nk_command_buffer* out, nk_rect scroll, int has_scrolling, float offset, float target, float step, float button_pixel_inc, const nk_style_scrollbar* style, nk_input* in, const nk_user_font* font);

/* selectable */
NK_LIB void nk_draw_selectable(nk_command_buffer* out, nk_flags state, const nk_style_selectable* style, nk_bool active, const nk_rect* bounds, const nk_rect* icon, const nk_image* img, nk_symbol_type sym, const char* string, int len, nk_flags align, const nk_user_font* font);
NK_LIB nk_bool nk_do_selectable(nk_flags* state, nk_command_buffer* out, nk_rect bounds, const char* str, int len, nk_flags align, nk_bool* value, const nk_style_selectable* style, const nk_input* in, const nk_user_font* font);
NK_LIB nk_bool nk_do_selectable_image(nk_flags* state, nk_command_buffer* out, nk_rect bounds, const char* str, int len, nk_flags align, nk_bool* value, const nk_image* img, const nk_style_selectable* style, const nk_input* in, const nk_user_font* font);

/* edit */
NK_LIB void nk_edit_draw_text(nk_command_buffer* out, const nk_style_edit* style, float pos_x, float pos_y, float x_offset, const char* text, int byte_len, float row_height, const nk_user_font* font, nk_color background, nk_color foreground, nk_bool is_selected);
NK_LIB nk_flags nk_do_edit(nk_flags* state, nk_command_buffer* out, nk_rect bounds, nk_flags flags, nk_plugin_filter filter, nk_text_edit* edit, const nk_style_edit* style, nk_input* in, const nk_user_font* font);

/* color-picker */
NK_LIB nk_bool nk_color_picker_behavior(nk_flags* state, const nk_rect* bounds, const nk_rect* matrix, const nk_rect* hue_bar, const nk_rect* alpha_bar, nk_colorf* color, const nk_input* in);
NK_LIB void nk_draw_color_picker(nk_command_buffer* o, const nk_rect* matrix, const nk_rect* hue_bar, const nk_rect* alpha_bar, nk_colorf col);
NK_LIB nk_bool nk_do_color_picker(nk_flags* state, nk_command_buffer* out, nk_colorf* col, nk_color_format fmt, nk_rect bounds, nk_vec2 padding, const nk_input* in, const nk_user_font* font);

/* property */
typedef enum {
  NK_PROPERTY_DEFAULT,
  NK_PROPERTY_EDIT,
  NK_PROPERTY_DRAG
} nk_property_status;
typedef enum {
  NK_FILTER_INT,
  NK_FILTER_FLOAT
} nk_property_filter;
typedef enum {
  NK_PROPERTY_INT,
  NK_PROPERTY_FLOAT,
  NK_PROPERTY_DOUBLE
} nk_property_kind;
typedef union {
  int i;
  float f;
  double d;
} nk_property;
typedef struct {
  nk_property_kind kind;
  nk_property value;
  nk_property min_value;
  nk_property max_value;
  nk_property step;
} nk_property_variant;
NK_LIB nk_property_variant nk_property_variant_int(int value, int min_value, int max_value, int step);
NK_LIB nk_property_variant nk_property_variant_float(float value, float min_value, float max_value, float step);
NK_LIB nk_property_variant nk_property_variant_double(double value, double min_value, double max_value, double step);

NK_LIB void nk_drag_behavior(nk_flags* state, const nk_input* in, nk_rect drag, nk_property_variant* variant, float inc_per_pixel);
NK_LIB void nk_property_behavior(nk_flags* ws, const nk_input* in, nk_rect property, nk_rect label, nk_rect edit, nk_rect empty, int* state, nk_property_variant* variant, float inc_per_pixel);
NK_LIB void nk_draw_property(nk_command_buffer* out, const nk_style_property* style, const nk_rect* bounds, const nk_rect* label, nk_flags state, const char* name, int len, const nk_user_font* font);
NK_LIB void nk_do_property(nk_flags* ws, nk_command_buffer* out, nk_rect property, const char* name, nk_property_variant* variant, float inc_per_pixel, char* buffer, int* len, int* state, int* cursor, int* select_begin, int* select_end, const nk_style_property* style, nk_property_filter filter, nk_input* in, const nk_user_font* font, nk_text_edit* text_edit, nk_button_behavior behavior);
NK_LIB void nk_make_property(nk_context* ctx, const char* name, nk_property_variant* variant, float inc_per_pixel, const nk_property_filter filter);

#ifdef NK_INCLUDE_FONT_BAKING

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#endif /* NK_INCLUDE_FONT_BAKING */

#endif
