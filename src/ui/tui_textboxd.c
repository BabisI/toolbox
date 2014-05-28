/*!
 * \file tui_textboxd.c
 * \brief
 *    A demonised text-box functionality.
 *
 * This file is part of toolbox
 *
 * Copyright (C) 2010-2014 Houtouridis Christos (http://www.houtouridis.net)
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
#include <ui/tuid.h>

// Static functions
static void _mk_caption (tuid_t *tuid, text_t cap);
static void _mk_frame (tuid_t *tuid, char* str);
static int _strcpy (char* to, const char* from, int size);

/*!
 * \brief
 *    Paints the Caption line in the frame buffer
 * \param  tuid   Pointer to the active tuid_t struct
 * \param  cap    Pointer to caption string
 * \return none
 */
static void _mk_caption (tuid_t *tuid, text_t cap)
{
   if (!tuid->frame_buffer.fb)
      return;
   // Clear ALL fb's caption first
   memset ((char*)&tuid->frame_buffer.fb[0], ' ', tuid->frame_buffer.c-1);
   tuid->frame_buffer.fb[tuid->frame_buffer.c-1] = 0; // Keep null termination at end of line

   // Print caption
   sprintf ((char*)&tuid->frame_buffer.fb[0], "%s", (char*)cap);
   tuid->frame_buffer.fb[strlen ((const char*)cap)] = ' ';
   /*
    * discard null termination inside frame buffer
    */
}

/*!
 * \brief
 *    Paints the frame in the frame buffer
 * \param  tuid   Pointer to the active tuid_t struct
 * \param  v      Value to print
 * \param  dec    The number of decimal points to use
 * \param  units  The units string to append.
 * \return none
 */
static void _mk_frame (tuid_t *tuid, char* str)
{
   #define _LINE(_l)    (tuid->frame_buffer.c*(_l))
   int line, offset=0;

   if (!tuid->frame_buffer.fb)
      return;
   // Clear fb's frame first
   for (line=1 ; line<tuid->frame_buffer.l ; ++line) {
      memset ((char*)&tuid->frame_buffer.fb[_LINE(line)], ' ', tuid->frame_buffer.c-1);
      tuid->frame_buffer.fb[_LINE(line+1)-1] = 0;
      /*
       * Keep null termination at end of each
       * frame buffer's line
       */
   }
   // Print text
   offset = sprintf ((char*)&tuid->frame_buffer.fb[_LINE(1)], ":%s<", str);
   // discard null termination inside frame buffer
   tuid->frame_buffer.fb[_LINE(1)+offset] = ' ';
   #undef _LINE
}

/*!
 * \brief
 *    Copy the from-string to to-string and stops at:
 *    - from string end
 *    - size limit
 *    - UI_TEXTBOX_SIZE size limit
 *    Whatever comes first.
 * \param   to    destination string
 * \param   from  source string
 * \param   size  the size limit
 * \return        The copied numbers.
 */
static int _strcpy (char* to, const char* from, int size)
{
   int i;
   for (i=0 ; from[i] && i<size && i<UI_TEXTBOX_SIZE; ++i)
      to[i] = from[i];
   to[i] = 0;
   return i;
}

/*!
 * \brief
 *    Creates a Textbox using lowercase, uppercase, digits and '-'
 *
 * \param   tuid     Pointer to the active tuid_t structure
 * \param   key      User input
 * \param   cap      The Value box caption
 * \param   str      Pointer to string
 * \param   size     The string's size
 * \param   ln       The language to use. (Currently unused).
 *
 * * \return  ui_return_t
 *    \arg  EXIT_RETURN    :  Indicates that function returns
 *    \arg  EXIT_STAY      :  Indicates that functions has not returned
 *
 * This function can create a text box.
 * While the function returns EXIT_STAY it is still in progress. When the function
 * is done returns EXIT_RETURN. This assumes that the caller must handle with return
 * status in order to continues call or not the function.
 *
 * Navigation
 * ==========================
 * UP       --    Increase the current character
 * DOWN     --    Decrease the current character
 * RIGHT    --    Deletes the last character.
 * LEFT     --    Save current character and go to next.
 * ESC      --    Returns the string "as is"
 */
ui_return_t tui_textboxd (tuid_t *tuid, int key, text_t cap, char* str, int8_t size)
{
   static int ev=1;
   static int8_t i=0;
   static char bf[UI_TEXTBOX_SIZE];

   if (ev) {
      ev=i=0;
      if (!*str) {
         // init string if needed
         _strcpy (bf, "A", 1);
      }
      else {
         // Copy and go to last character
         i = _strcpy (bf, str, size) - 1;
      }
      _mk_caption (tuid, cap);
   }

   //Navigating
   if (key == tuid->keys.UP)
      // Increment character
      do
         bf[i]++;
      while ( !isalnum ((int)str[i]) &&
              str[i]!='-' &&
              str[i]!='_' );
   if (key == tuid->keys.DOWN)
      // Decrement character
      do
         bf[i]--;
      while ( !isalnum ((int)str[i]) &&
              str[i]!='-' &&
              str[i]!='_' );
   if (key == tuid->keys.LEFT) {
      // Manual backspace
      bf[i] = 0;
      if (--i<0) {
         ev=1;
         return EXIT_RETURN;
      }
   }
   if (key == tuid->keys.RIGHT || key == tuid->keys.ENTER) {
      // Next or OK
      if (++i>=size) {
         // At the end of the string we return the string
         ev=1;
         _strcpy (str, bf, size);
         return EXIT_RETURN;
      }
      // Copy the previous character
      if (!bf[i])  bf[i] = bf[i-1];
      bf[i+1] = 0;
   }
   if (key == tuid->keys.ENTER_L) {
      // Return this string
      ev=1;
      _strcpy (str, bf, size);
      return EXIT_RETURN;
   }
   if (key == tuid->keys.ESC) {
      // return no string
      ev=1;
      return EXIT_RETURN;
   }

   // Paint the screen
   _mk_frame (tuid, bf);
   return EXIT_STAY;
}

