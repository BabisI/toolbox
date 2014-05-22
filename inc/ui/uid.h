/*!
 * \file ui.h
 * \brief
 *    A small footprint ui library
 *
 * Copyright (C) 2010-2014 Houtouridis Christos <houtouridis.ch@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author:     Houtouridis Christos <houtouridis.ch@gmail.com>
 *
 */

#ifndef  __uid_h__
#define  __uid_h__

#ifdef   __cplusplus
extern "C" {
#endif

#include <std/printf.h>
#include <std/stime.h>
#include <stdint.h>
#include <ctype.h>

/* ========================== User Defines ============================ */
#define  UI_TIMEBOX_SIZE      (12)
#define  UI_TEXTBOX_SIZE      (14)


/* ========================== General Defines ============================ */
#define  UI_NUM_OF_LANGUAGES     (2)

#define  UI_TIME_SS              (0x01)
#define  UI_TIME_MM              (0x02)
#define  UI_TIME_HH              (0x04)
#define  UI_TIME_DD              (0x08)

/* ========================== UI Data types ============================ */
typedef char* text_t;

typedef enum
{
   EXIT_STAY=0,
   EXIT_RETURN
}ui_return_t;

typedef volatile struct
{
   int   UP;
   int   DOWN;
   int   ENTER;
   int   ENTER_L;
   int   RIGHT;
   int   LEFT;
   int   ESC;
}ui_keys_t;
extern ui_keys_t   ui_keys;

typedef enum
{
   LANG_EN=0,
   LANG_GR
}Lang_en;




/* ================    Exported Functions    ======================*/
extern void ui_print_ctrl (char ch);
extern void ui_print_caption (text_t cap);
extern void ui_print_box (text_t box);
extern void ui_print_frame (text_t fr, size_t step);
extern int ui_getkey (uint8_t wait);


ui_return_t ui_valuebox (int key, text_t cap, text_t units, float up, float down, float step, int dec, float *value);
ui_return_t ui_timebox (int key, text_t cap, uint8_t frm, time_t up, time_t down, time_t step, time_t *value);
ui_return_t ui_textbox (int key, text_t cap, char* str, int8_t size, Lang_en ln);


#ifdef  __cplusplus
}
#endif

#endif //#ifndef  __ui_h__
