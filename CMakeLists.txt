#=============================================================================
# Setup project
#=============================================================================
project(d2_opencode12 CXX)
cmake_minimum_required(VERSION 2.8)


#=============================================================================
# Setup CMake options
#=============================================================================
include(CMakeDependentOption)
option(ENABLE_PEDANTIC "Compile with pedantic enabled." ON)
option(ENABLE_WERROR   "Fail and stop if a warning is triggered." OFF)
option(ENABLE_LIBCXX   "Enable -stdlib=libc++ if the compiler supports it." ON)


#=============================================================================
# Check available compiler flags
#=============================================================================
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-pedantic                    HAS_PEDANTIC_FLAG)
check_cxx_compiler_flag(-std=c++0x                   HAS_STDCXX0X_FLAG)
check_cxx_compiler_flag(-stdlib=libc++               HAS_STDLIB_LIBCXX_FLAG)
check_cxx_compiler_flag(-W                           HAS_W_FLAG)
check_cxx_compiler_flag(-Wall                        HAS_WALL_FLAG)
check_cxx_compiler_flag(-Werror                      HAS_WERROR_FLAG)
check_cxx_compiler_flag(-Wextra                      HAS_WEXTRA_FLAG)
check_cxx_compiler_flag(-Wno-long-long               HAS_WNO_LONG_LONG_FLAG)
check_cxx_compiler_flag(-Wno-unused-local-typedefs   HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG)
check_cxx_compiler_flag(-Wno-unused-parameter        HAS_WNO_UNUSED_PARAMETER_FLAG)
check_cxx_compiler_flag(-Wwrite-strings              HAS_WWRITE_STRINGS_FLAG)
check_cxx_compiler_flag(-ftemplate-backtrace-limit=0 HAS_FTEMPLATE_BACKTRACE_LIMIT_0)


#=============================================================================
# Setup warning flags
#=============================================================================
macro(append_if condition lst var)
    if (${condition})
        list(APPEND ${lst} ${var})
    endif()
endmacro()
append_if(HAS_WALL_FLAG                        CXX_WARNING_FLAGS -Wall)
append_if(HAS_WEXTRA_FLAG                      CXX_WARNING_FLAGS -Wextra)
append_if(HAS_W_FLAG                           CXX_WARNING_FLAGS -W)
append_if(HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG   CXX_WARNING_FLAGS -Wno-unused-local-typedefs)
append_if(HAS_WNO_UNUSED_PARAMETER_FLAG        CXX_WARNING_FLAGS -Wno-unused-parameter)
append_if(HAS_WWRITE_STRINGS_FLAG              CXX_WARNING_FLAGS -Wwrite-strings)
append_if(HAS_WNO_LONG_LONG_FLAG               CXX_WARNING_FLAGS -Wno-long-long)
append_if(HAS_FTEMPLATE_BACKTRACE_LIMIT_0      CXX_WARNING_FLAGS -ftemplate-backtrace-limit=0)

if (ENABLE_WERROR)
    append_if(HAS_WERROR_FLAG  CXX_WARNING_FLAGS -Werror)
    append_if(HAS_WX_FLAG      CXX_WARNING_FLAGS -WX)
endif()

if (ENABLE_PEDANTIC)
    append_if(HAS_PEDANTIC_FLAG CXX_WARNING_FLAGS -pedantic)
endif()


#=============================================================================
# Setup feature flags
#=============================================================================
if (HAS_STDCXX0X_FLAG)
    list(APPEND CXX_FEATURE_FLAGS -std=c++0x)
endif()

if (ENABLE_LIBCXX AND HAS_STDLIB_LIBCXX_FLAG)
    list(APPEND CXX_FEATURE_FLAGS -stdlib=libc++)
    # the flag has to be passed to the linker too.
    set(CMAKE_EXE_LINKER_FLAGS -stdlib=libc++)
    set(CMAKE_SHARED_LINKER_FLAGS -stdlib=libc++)
    set(CMAKE_MODULE_LINKER_FLAGS -stdlib=libc++)
endif()

# This is the only place where add_definitions is called. Other properties
# are set on a per-target basis.
add_definitions(
    ${CXX_WARNING_FLAGS}
    ${CXX_FEATURE_FLAGS}
)


#=============================================================================
# Setup the code
#=============================================================================
find_package(Boost 1.53 REQUIRED thread system)


file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(main ${SOURCES})
target_link_libraries(main
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)
include_directories(${Boost_INCLUDE_DIRS})
