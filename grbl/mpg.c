/*
  mpg.c - code for switching imput to secondary input stream from MPG pendant
  Part of Grbl

  Copyright (c) 2018 Terje Io
  
  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/
  
#include "grbl.h"

void mpg_select (uint8_t mpg_mode)
{
  if(mpg_mode == sys.mpg_mode || (mpg_mode && sys.state != STATE_IDLE))
   return;

  serial_select(mpg_mode);

  // Report WCO on first status report request from MPG processor
  if(mpg_mode)
	sys.report_wco_counter = 1;

  // Force a realtime status report
  system_set_exec_state_flag(EXEC_STATUS_REPORT);

  sys.mpg_mode =  mpg_mode;
}

void mpg_init(void)
{
  MPG_DDR &= ~(MPG_MASK);  // Set as input pin
  MPG_PORT |= (MPG_MASK);  // Enable internal pull-up resistors.

  MPG_PCMSK |= MPG_IMASK;  // Enable specific pins of the Pin Change Interrupt
  PCICR |= (1 << MPG_INT); // Enable Pin Change Interrupt

  mpg_select((MPG_PIN & MPG_MASK) == 0); // Set initial mode to MPG?
}

ISR(MPG_INT_vect)
{
  mpg_select((MPG_PIN & MPG_MASK) == 0); // TODO: implement as toggle switch, debouncing?
}  
