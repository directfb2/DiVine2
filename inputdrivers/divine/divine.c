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

#define DFB_INPUTDRIVER_HAS_AXIS_INFO

#include <core/input_driver.h>
#include <direct/thread.h>

D_DEBUG_DOMAIN( DiVine, "Input/DiVine", "DiVine Input Driver" );

DFB_INPUT_DRIVER( divine )

/**********************************************************************************************************************/

#define PIPE_PATH "/tmp/divine"

typedef struct {
     CoreInputDevice *device;

     int              fd;

     DirectThread    *thread;
} DiVineData;

/**********************************************************************************************************************/

static void *
divine_event_thread( DirectThread *thread,
                     void         *arg )
{
     DiVineData *data = arg;

     D_DEBUG_AT( DiVine, "%s()\n", __FUNCTION__ );

     while (1) {
          DFBInputEvent evt;
          fd_set        set;
          int           status;

          FD_ZERO( &set );
          FD_SET( data->fd, &set );

          status = select( data->fd + 1, &set, NULL, NULL, NULL );
          if (status < 0 && errno != EINTR)
               break;

          if (read( data->fd, &evt, sizeof(DFBInputEvent) ) == sizeof(DFBInputEvent)) {
               dfb_input_dispatch( data->device, &evt );
          }
          else {
               if (!strcmp( (const char*) &evt, "STOP" ))
                    break;
          }
     }

     D_DEBUG_AT( DiVine, "DiVine Event thread terminated\n" );

     return NULL;
}

/**********************************************************************************************************************/

static int
driver_get_available()
{
     int  fd;

     /* Create the pipe if it does not already exist. */
     if (mkfifo( PIPE_PATH, 0660 ) && errno != EEXIST) {
          D_PERROR( "Input/DiVine: Could not create pipe '%s'!\n", PIPE_PATH );
          return 0;
     }

     /* Open pipe. */
     fd = open( PIPE_PATH, O_RDONLY | O_NONBLOCK );
     if (fd < 0)
          return 0;

     close( fd );

     return 1;
}

static void
driver_get_info( InputDriverInfo *driver_info )
{
     driver_info->version.major = 0;
     driver_info->version.minor = 1;

     snprintf( driver_info->name,   DFB_INPUT_DRIVER_INFO_NAME_LENGTH,   "DiVine" );
     snprintf( driver_info->vendor, DFB_INPUT_DRIVER_INFO_VENDOR_LENGTH, "DirectFB" );
}

static DFBResult
driver_open_device( CoreInputDevice  *device,
                    unsigned int      number,
                    InputDeviceInfo  *device_info,
                    void            **driver_data )
{
     DiVineData *data;
     int         fd;

     /* Create the pipe if it does not already exist. */
     if (mkfifo( PIPE_PATH, 0660 ) && errno != EEXIST) {
          D_PERROR( "Input/DiVine: Could not create pipe '%s'!\n", PIPE_PATH );
          return 0;
     }

     /* Open pipe. */
     fd = open( PIPE_PATH, O_RDWR | O_NONBLOCK );
     if (fd < 0) {
          D_PERROR( "Input/DiVine: Could not open pipe '%s'!\n", PIPE_PATH );
          return DFB_INIT;
     }

     /* Fill device information. */
     device_info->prefered_id   = DIDID_ANY;
     device_info->desc.type     = DIDTF_KEYBOARD | DIDTF_MOUSE | DIDTF_JOYSTICK | DIDTF_REMOTE | DIDTF_VIRTUAL;
     device_info->desc.caps     = DICAPS_ALL;
     device_info->desc.max_axis = DIAI_LAST;
     snprintf( device_info->desc.name,   DFB_INPUT_DEVICE_DESC_NAME_LENGTH,   "Virtual Input" );
     snprintf( device_info->desc.vendor, DFB_INPUT_DEVICE_DESC_VENDOR_LENGTH, "DiVine" );

     /* Allocate and fill private data. */
     data = D_CALLOC( 1, sizeof(DiVineData) );
     if (!data) {
          close( fd );
          return D_OOM();
     }

     data->device = device;
     data->fd     = fd;

     /* Start divine event thread */
     data->thread = direct_thread_create( DTT_INPUT, divine_event_thread, data, "DiVine Event" );

     *driver_data = data;

     return DFB_OK;
}

static DFBResult
driver_get_keymap_entry( CoreInputDevice           *device,
                         void                      *driver_data,
                         DFBInputDeviceKeymapEntry *entry )
{
     return DFB_UNSUPPORTED;
}

static void
driver_close_device( void *driver_data )
{
     DiVineData *data = driver_data;

     /* Terminate the divine event thread. */
     write( data->fd, "STOP", 5 );

     direct_thread_join( data->thread );
     direct_thread_destroy( data->thread );

     close( data->fd );

     unlink( PIPE_PATH );

     D_FREE( data );
}

/**********************************************************************************************************************
 ********************************* Axis info function *****************************************************************
 **********************************************************************************************************************/

static DFBResult
driver_get_axis_info( CoreInputDevice              *device,
                      void                         *driver_data,
                      DFBInputDeviceAxisIdentifier  axis,
                      InputDeviceAxisInfo          *ret_info )
{
     ret_info->flags   = IDAIF_ABS_MIN | IDAIF_ABS_MAX;
     ret_info->abs_min = 0;
     ret_info->abs_max = 65535;

     return DFB_OK;
}
