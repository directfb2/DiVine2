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

#ifndef __DIVINE_H__
#define __DIVINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <directfb.h>

/*
 * Main interface of DiVine.
 */
D_DECLARE_INTERFACE( IDiVine )

/**********************************************************************************************************************/

#define DIVINE_API

/**********************************************************************************************************************/

/*
 * Parses the command-line and initializes some variables.
 * You absolutely need to call this before doing anything else.
 * Removes all options used by DiVine from argv.
 */
DFBResult DIVINE_API DiVineInit(
     int                                    *argc,               /* pointer to main()'s argc */
     char                                  **argv[]              /* pointer to main()'s argv */
);

/*
 * Creates the main interface.
 */
DFBResult DIVINE_API DiVineCreate(
     IDiVine                               **ret_interface       /* pointer to the created interface */
);

/**********************************************************************************************************************/

/***********
 * IDiVine *
 ***********/

/*
 * IDiVine is the main interface. It can be retrieved by a
 * call to DiVineCreate().
 */
D_DEFINE_INTERFACE( IDiVine,

   /** Events **/

     /*
      * Send an input event.
      */
     DFBResult (*SendEvent) (
          IDiVine                           *thiz,
          const DFBInputEvent               *event
     );

     /*
      * Send an input key symbol.
      */
     DFBResult (*SendSymbol) (
          IDiVine                           *thiz,
          DFBInputDeviceKeySymbol            symbol
     );
);

#ifdef __cplusplus
}
#endif

#endif
