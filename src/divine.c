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

#include <direct/direct.h>
#include <idivine.h>
#include <misc/conf.h>

D_DEBUG_DOMAIN( DiVine_Main, "DiVine/Main", "DiVine Main Functions" );

/**********************************************************************************************************************/

DFBResult
DiVineInit( int   *argc,
            char **argv[] )
{
     DFBResult ret;

     ret = dfb_config_init( argc, argv );
     if (ret)
          return ret;

     return DFB_OK;
}

DFBResult
DiVineCreate( IDiVine **ret_interface )
{
     DFBResult  ret;
     IDiVine   *divine;

     D_DEBUG_AT( DiVine_Main, "%s( %p )\n", __FUNCTION__, ret_interface );

     if (!dfb_config) {
          D_ERROR( "DiVine/Main: DiVineInit() has to be called before DiVineCreate()!\n" );
          return DFB_INIT;
     }

     if (!ret_interface)
          return DFB_INVARG;

     direct_initialize();

     DIRECT_ALLOCATE_INTERFACE( divine, IDiVine );

     ret = IDiVine_Construct( divine );
     if (ret)
          return ret;

     *ret_interface = divine;

     return DFB_OK;
}
