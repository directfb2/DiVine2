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

#include <core_divine.h>

D_DEBUG_DOMAIN( CoreDiVine_Main, "CoreDiVine/Main", "DiVine Core" );

/**********************************************************************************************************************/

struct __DV_CoreDiVine {
     int fd; /* file descriptor of the connection (pipe) */
};

/**********************************************************************************************************************/

CoreDiVine *
divine_open( const char *path )
{
     int         fd;
     CoreDiVine *divine;

     D_DEBUG_AT( CoreDiVine_Main, "%s()\n", __FUNCTION__ );

     /* Open pipe. */
     fd = open( path, O_WRONLY );
     if (fd < 0) {
          D_PERROR( "CoreDiVine: Could not open pipe '%s'!\n", path );
          return NULL;
     }

     /* Allocate connection object. */
     divine = D_CALLOC( 1, sizeof(CoreDiVine) );
     if (!divine) {
          D_OOM();
          return NULL;
     }

     /* Fill out connection information. */
     divine->fd = fd;

     return divine;
}

void
divine_send_event( CoreDiVine          *divine,
                   const DFBInputEvent *event )
{
     D_DEBUG_AT( CoreDiVine_Main, "%s()\n", __FUNCTION__ );

     /* Write event to pipe. */
     write( divine->fd, event, sizeof(DFBInputEvent) );
}

void
divine_send_symbol( CoreDiVine              *divine,
                    DFBInputDeviceKeySymbol  symbol )
{
     DFBInputEvent event;

     D_DEBUG_AT( CoreDiVine_Main, "%s()\n", __FUNCTION__ );

     /* Construct 'press' event. */
     event.flags      = DIEF_KEYSYMBOL;
     event.type       = DIET_KEYPRESS;
     event.key_symbol = symbol;

     /* Write 'press' event to pipe. */
     write( divine->fd, &event, sizeof(DFBInputEvent) );

     /* Turn into 'release' event. */
     event.type = DIET_KEYRELEASE;

     /* Write 'release' event to pipe. */
     write( divine->fd, &event, sizeof(DFBInputEvent) );
}

void
divine_close( CoreDiVine *divine )
{
     D_DEBUG_AT( CoreDiVine_Main, "%s()\n", __FUNCTION__ );

     /* Close pipe. */
     close( divine->fd );

     /* Free connection object. */
     D_FREE( divine );
}
