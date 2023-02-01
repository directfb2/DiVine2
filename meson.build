#  This file is part of DirectFB.
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

project('DiVine2', 'c',
        version: '2.0.0',
        meson_version: '>= 0.61',
        default_options: 'buildtype=release')

divine_version = meson.project_version()
divine_major_version = divine_version.split('.')[0].to_int()
divine_minor_version = divine_version.split('.')[1].to_int()
divine_micro_version = divine_version.split('.')[2].to_int()

if get_option('default_library') == 'both'
  error('''Choose library type between 'shared' or 'static'.''')
elif get_option('default_library') == 'shared'
  libsuffix = '.so'
else
  libsuffix = '.a'
endif

directfb_dep = dependency('directfb-internal')

moduledir = directfb_dep.get_variable(pkgconfig: 'moduledir')

divine_inc = include_directories('include')

pkgconfig = import('pkgconfig')

subdir('include')
subdir('inputdrivers/divine')
subdir('src')
