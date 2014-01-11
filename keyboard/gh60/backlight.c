/*
Copyright 2013 Kai Ryu <kai1103@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "backlight.h"

#ifdef GH60_REV_CHN
static const uint8_t backlight_table[] PROGMEM = {
    0, 16, 128, 255
};

/* Backlight pin configuration
 * PWM: PB7
 */
void backlight_set(uint8_t level)
{
    if (level > 0) {
        // Turn on PWM
        cli();
        DDRB |= (1<<PB6);
        TCCR1A |= ( (1<<WGM10) | (1<<COM1B1) );
        TCCR1B |= ( (1<<CS11) | (1<<CS10) );
        sei();
        // Set PWM
        OCR1B = pgm_read_byte(&backlight_table[level]);
    }
    else {
        // Turn off PWM
        cli();
        DDRB &= ~(1<<PB6);
        TCCR1A &= ~( (1<<WGM10) | (1<<COM1B1) );
        TCCR1B &= ~( (1<<CS11) | (1<<CS10) );
        sei();
        // Set PWM
        OCR1B = 0;
    }
}
#else
void backlight_set(uint8_t level)
{
    if (level > 0) {
        DDRF  |=  (1<<PF7 | 1<<PF6 | 1<<PF5 | 1<<PF4);
        PORTF &= ~(1<<PF7 | 1<<PF6 | 1<<PF5 | 1<<PF4);
    }
    else {
        DDRF  &= ~(1<<PF7 | 1<<PF6 | 1<<PF5 | 1<<PF4);
    }
}
#endif