#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <lcom/lcf.h>

int (map_VRAM)(uint16_t mode);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_color)(uint16_t x, uint16_t y, uint32_t color);
int (draw_xpm)(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y);
vbe_mode_info_t (get_mode_inf)();
unsigned (get_hres)();
unsigned (get_vres)();
unsigned (get_bits_per_pixel)();
unsigned (get_bytes_per_pixel)();
char* (get_video_mem)();
void (swap_buffer)();
char* (get_second_buffer)();
void (free_second_buffer)();

#endif
