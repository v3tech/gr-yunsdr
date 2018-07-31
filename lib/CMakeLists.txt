# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR} ${LIBYUNSDR_INCLUDE_DIRS} ${LIBYUNSDR_SS_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND yunsdr_sources
    yunsdr_source_impl.cc
    yunsdr_sink_impl.cc
)

list(APPEND yunsdr_ss_sources
	yunsdr_ss_source_impl.cc
    yunsdr_ss_sink_impl.cc
)

set(yunsdr_sources "${yunsdr_sources}" PARENT_SCOPE)
set(yunsdr_ss_sources "${yunsdr_ss_sources}" PARENT_SCOPE)
if(NOT yunsdr_sources AND NOT yunsdr_ss_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT yunsdr_sources AND NOT yunsdr_ss_sources)

if ("${PC_GNURADIO_RUNTIME_VERSION}" VERSION_LESS 3.8.0)
	set(GR_IS_VERSION_3_7_OR_LESS ON)
endif()

option(COMPAT_GR_3_7_OR_LESS "Compile for GNU Radio version 3.7 or less" ${GR_IS_VERSION_3_7_OR_LESS})
if (COMPAT_GR_3_7_OR_LESS)
	add_definitions(-DGR_VERSION_3_7_OR_LESS)
endif()

add_library(gnuradio-yunsdr SHARED ${yunsdr_sources})
target_link_libraries(gnuradio-yunsdr
	${Boost_LIBRARIES}
	${LIBYUNSDR_LIBRARIES}
	${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-yunsdr PROPERTIES
	VERSION ${GR_YUNSDR_VERSION}
	SOVERSION ${GR_YUNSDR_VERSION_MAJOR}
	DEFINE_SYMBOL "gnuradio_yunsdr_EXPORTS"
)

add_library(gnuradio-yunsdr-ss SHARED ${yunsdr_ss_sources})
target_link_libraries(gnuradio-yunsdr-ss
	${Boost_LIBRARIES}
	${LIBYUNSDR_SS_LIBRARIES}
	${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-yunsdr-ss PROPERTIES
	VERSION ${GR_YUNSDR_VERSION}
	SOVERSION ${GR_YUNSDR_VERSION_MAJOR}
	DEFINE_SYMBOL "gnuradio_yunsdr_EXPORTS"
)

if(APPLE)
    set_target_properties(gnuradio-yunsdr PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
    set_target_properties(gnuradio-yunsdr-ss PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-yunsdr
	LIBRARY DESTINATION ${GR_LIBRARY_DIR} # .so/.dylib file
	ARCHIVE DESTINATION ${GR_LIBRARY_DIR} # .lib file
	RUNTIME DESTINATION ${GR_RUNTIME_DIR} # .dll file
)

install(TARGETS gnuradio-yunsdr-ss
	LIBRARY DESTINATION ${GR_LIBRARY_DIR} # .so/.dylib file
	ARCHIVE DESTINATION ${GR_LIBRARY_DIR} # .lib file
	RUNTIME DESTINATION ${GR_RUNTIME_DIR} # .dll file
)

########################################################################
# Setup the include and linker paths
########################################################################
#include_directories(
#	${GR_YUNSDR_INCLUDE_DIRS}
#   ${GNURADIO_RUNTIME_INCLUDE_DIRS}
#   ${Boost_INCLUDE_DIRS}
