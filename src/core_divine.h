/*
   This file is part of DirectFB.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*/

#ifndef __CORE_DIVINE_H__
#define __CORE_DIVINE_H__

#include <coretypes_divine.h>

/**********************************************************************************************************************/

/*
 * Returns the DiVine connection object (connection to the input driver by opening the pipe specified by 'path').
 */
CoreDiVine *divine_open       ( const char              *path );

/*
 * Sends an input event.
 */
void        divine_send_event ( CoreDiVine              *divine,
                                const DFBInputEvent     *event );

/*
 * Sends a press and a release event for the specified symbol.
 */
void        divine_send_symbol( CoreDiVine              *divine,
                                DFBInputDeviceKeySymbol  symbol );

/*
 * Closes the pipe and destroys the connection object.
 */
void        divine_close      ( CoreDiVine              *divine );

#endif
