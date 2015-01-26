#
# firebird (trunk)
#

###############################################################################
#
# cmake settings
#
###############################################################################

cmake_minimum_required(VERSION 2.8.8)

# In-source builds are not possible and so disabled.
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "CMake generation for firebird is not possible within the source directory!"
        "\n Remove the CMakeCache.txt file and try again from another folder, e.g.:"
        "\n "
        "\n rm CMakeCache.txt"
        "\n mkdir build"
        "\n cd build"
        "\n cmake .."
    )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/builds/cmake)

if (NATIVE_BUILD_DIR)
    get_filename_component(NATIVE_BUILD_DIR ${NATIVE_BUILD_DIR} ABSOLUTE)
else()
    set(NATIVE_BUILD_DIR ${CMAKE_BINARY_DIR})
endif()

# Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake Targets")

###############################################################################
#
# project settings
#
###############################################################################

project("firebird" C CXX)

#######################################

set(output_dir ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${output_dir})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${output_dir})

if (MSVC OR XCODE)
    set(output_dir ${output_dir}/$<CONFIG>)
endif()

if (XCODE)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${conf} conf2)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${conf2} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${conf}/bin)
    endforeach()
elseif (UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${output_dir}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${output_dir}/bin)
endif()

###############################################################################
#
# configure
#
###############################################################################

include(Configure)

if (FREEBSD)
    # temporary
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
endif()

set(FB_PREFIX ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME})
set(FB_IPC_NAME "FirebirdIPI")
set(FB_LOGFILENAME "firebird.log")
set(FB_PIPE_NAME "interbas")
set(FB_SERVICE_NAME "gds_db")
set(FB_SERVICE_PORT 3050)

if (WIN32)
    set(FB_PREFIX "c:\\\\Program Files\\\\Firebird\\\\")
    set(FB_IPC_NAME "FIREBIRD")
endif()

set(AUTOCONFIG_SRC ${CMAKE_SOURCE_DIR}/src/include/gen/autoconfig.h.in)
set(AUTOCONFIG ${CMAKE_BINARY_DIR}/src/include/gen/autoconfig.h)
configure_file(${AUTOCONFIG_SRC} ${AUTOCONFIG} @ONLY)

###############################################################################
#
# compiler & linker
#
###############################################################################

add_definitions(-DDEV_BUILD)

if (WIN32)
    set(OS_DIR win32)
    set(VERSION_RC ${CMAKE_SOURCE_DIR}/src/jrd/version.rc)

    if (MSVC)
        set(disable_msvc_warnings "/wd4996")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP ${disable_msvc_warnings}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP ${disable_msvc_warnings}")
    endif(MSVC)

    set(LIB_Ws2_32 Ws2_32)
    set(LIB_comctl32 comctl32)
    set(LIB_mpr mpr)
    set(LIB_version version)
endif(WIN32)

if (MINGW)
    add_definitions(-D_WIN32_WINNT=0x0600)
endif()

if (UNIX)
    set(OS_DIR posix)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

    if (NOT CMAKE_CROSSCOMPILING)
        set(LIB_readline readline)
    endif()
    if (NOT FREEBSD)
        set(LIB_dl dl)
    endif()
endif()

if (CLANG OR IOS)
    set(LIB_iconv iconv)
endif()

if (FREEBSD)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif()

if (APPLE)
    set(OS_DIR darwin)

    include_directories(/opt/local/include)
    if (NOT CMAKE_CROSSCOMPILING)
        link_directories(/opt/local/lib)
    endif()

    find_library(LIB_CoreFoundation CoreFoundation)
elseif (UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

###############################################################################
#
# pre build
#
###############################################################################

if (WIN32)
    # icu
    set(ICU_EXTRACT ${CMAKE_CURRENT_SOURCE_DIR}/extern/icu/icu.exe -y)
    #message("Extracting pre-built ICU")
    execute_process(COMMAND ${ICU_EXTRACT})

    # zlib
    set(ZLIB_EXTRACT ${CMAKE_CURRENT_SOURCE_DIR}/extern/zlib/zlib.exe -y)
    #message("Extracting pre-built zlib")
    execute_process(COMMAND ${ZLIB_EXTRACT})
endif()

if (UNIX)    
    file(GLOB vers_src "${CMAKE_SOURCE_DIR}/builds/posix/*.vers")
    foreach(f ${vers_src})
        get_filename_component(name ${f} NAME)
        set(name ${CMAKE_BINARY_DIR}/builds/posix/${name})

        if (AIX)
            file(WRITE ${name} "#!\n")
        elseif (LINUX)
            file(WRITE ${name} "{\nglobal:\n")
        endif()

        file(STRINGS ${f} strings)
        foreach(s ${strings})
            string(REGEX REPLACE "#.*$" "" s "${s}")
            string(STRIP "${s}" s)
            if (NOT "${s}" STREQUAL "")
                if (AIX)
                    file(APPEND ${name} "\t${s}\n")
                elseif (APPLE)
                    file(APPEND ${name} "\t_${s}\n")
                elseif (HPUX)
                    file(APPEND ${name} "+e ${s}\n")
                else()
                    file(APPEND ${name} "\t${s};\n")
                endif()
            endif()
        endforeach()

        if (LINUX)
            file(APPEND ${name} "local:\n\t*;\n};\n")
        endif()
    endforeach()
endif()


###############################################################################
#
# build
#
###############################################################################

include(BuildFunctions)

crosscompile_prebuild_steps()

include_directories("extern/libtommath")
include_directories("extern/icu/include")
include_directories("extern/zlib")

include_directories("src/include")
include_directories("src/include/gen")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/src/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/src/include/gen")

########################################
# EXECUTABLE btyacc
########################################

file(GLOB btyacc_src "extern/btyacc/*.c" "extern/btyacc/*.h")

if (NOT CMAKE_CROSSCOMPILING)

add_executable          (btyacc ${btyacc_src})
project_group           (btyacc Extern)
set_output_directory    (btyacc . FORCE)

endif() # if (NOT CMAKE_CROSSCOMPILING)


########################################
# LIBRARY btyacc
########################################

file(GLOB libtommath_src "extern/libtommath/*.c" "extern/libtommath/*.h")

add_library             (libtommath ${libtommath_src})
project_group           (libtommath Extern)

########################################

add_subdirectory("examples")
add_subdirectory("src")

###############################################################################

