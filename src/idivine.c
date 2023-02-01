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
#include <direct/direct.h>
#include <idivine.h>

D_DEBUG_DOMAIN( DiVine, "IDiVine", "IDiVine Interface" );

/**********************************************************************************************************************/

#define PIPE_PATH "/tmp/divine"

/*
 * private data struct of IDiVine
 */
typedef struct {
     int     ref;        /* reference counter */

     CoreDiVine *divine; /* the core object */
} IDiVine_data;

/**********************************************************************************************************************/

static void
IDiVine_Destruct( IDiVine *thiz )
{
     IDiVine_data *data = thiz->priv;

     D_DEBUG_AT( DiVine, "%s( %p )\n", __FUNCTION__, thiz );

     divine_close( data->divine );

     DIRECT_DEALLOCATE_INTERFACE( thiz );

     direct_shutdown();
}

static DirectResult
IDiVine_AddRef( IDiVine *thiz )
{
     DIRECT_INTERFACE_GET_DATA( IDiVine )

     D_DEBUG_AT( DiVine, "%s( %p )\n", __FUNCTION__, thiz );

     data->ref++;

     return DFB_OK;
}

static DirectResult
IDiVine_Release( IDiVine *thiz )
{
     DIRECT_INTERFACE_GET_DATA( IDiVine )

     D_DEBUG_AT( DiVine, "%s( %p )\n", __FUNCTION__, thiz );

     if (--data->ref == 0)
          IDiVine_Destruct( thiz );

     return DFB_OK;
}

static DFBResult
IDiVine_SendEvent( IDiVine             *thiz,
                   const DFBInputEvent *event )
{
     DIRECT_INTERFACE_GET_DATA( IDiVine )

     D_DEBUG_AT( DiVine, "%s( %p, %p )\n", __FUNCTION__, thiz, event );

     divine_send_event( data->divine, event );

     return DFB_OK;
}

static DFBResult
IDiVine_SendSymbol( IDiVine                 *thiz,
                    DFBInputDeviceKeySymbol  symbol )
{
     DIRECT_INTERFACE_GET_DATA( IDiVine )

     D_DEBUG_AT( DiVine, "%s( %p, 0x%08x )\n", __FUNCTION__, thiz, symbol );

     divine_send_symbol( data->divine, symbol );

     return DFB_OK;
}

DFBResult
IDiVine_Construct( IDiVine *thiz )
{
     DIRECT_ALLOCATE_INTERFACE_DATA( thiz, IDiVine )

     D_DEBUG_AT( DiVine, "%s( %p )\n", __FUNCTION__, thiz );

     data->divine = divine_open( PIPE_PATH );
     if (!data->divine) {
          DIRECT_DEALLOCATE_INTERFACE( thiz );
          return DFB_FAILURE;
     }

     data->ref = 1;

     thiz->AddRef     = IDiVine_AddRef;
     thiz->Release    = IDiVine_Release;
     thiz->SendEvent  = IDiVine_SendEvent;
     thiz->SendSymbol = IDiVine_SendSymbol;

     return DFB_OK;
}
