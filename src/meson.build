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

divine_sources = [
  'core_divine.c',
  'divine.c',
  'idivine.c'
]

libdivine = library('divine-@0@.@1@'.format(divine_major_version, divine_minor_version),
                    divine_sources,
                    include_directories: divine_inc,
                    dependencies: directfb_dep,
                    version: '0.0.@0@'.format(divine_micro_version),
                    install: true)

install_symlink('libdivine' + libsuffix,
                pointing_to: 'libdivine-@0@.@1@'.format(divine_major_version, divine_minor_version) + libsuffix,
                install_dir: join_paths(get_option('prefix'), get_option('libdir')))

pkgconfig.generate(filebase: 'divine',
                   name: 'DiVine',
                   description: 'DirectFB Virtual input extension',
                   requires: 'directfb',
                   libraries: '-L${libdir} -ldivine',
                   subdirs: 'divine')
