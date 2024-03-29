cmake_minimum_required(VERSION 3.18)

project(Fruity2D VERSION 0.3.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED YES)

# Define the 'Profile' build-type and set default to 'Release' for
# non-multi-config generators
get_property(isMultiConfig GLOBAL
        PROPERTY GENERATOR_IS_MULTI_CONFIG
)
if (isMultiConfig)
        if (NOT "Profile" IN_LIST CMAKE_CONFIGURATION_TYPES)
                list(APPEND CMAKE_CONFIGURATION_TYPES Profile)
        endif()
else()
        set(allowedBuildTypes Debug Release Profile)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
                STRINGS "${allowedBuildTypes}"
        )
        if (NOT CMAKE_BUILD_TYPE)
                set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
        elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowedBuildTypes)
                message(FATAL_ERROR
                        "Unknown build type: ${CMAKE_BUILD_TYPE}"
                )
        endif()
endif()

# Profile build settings
set(CMAKE_C_FLAGS_PROFILE               "-pg -g -O2" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_PROFILE      "-pg" CACHE STRING "" FORCE)

# General compilation flags
add_compile_options(-Wall -Wextra -Werror)

include(GNUInstallDirs)

if(APPLE)
	include_directories(${CMAKE_INSTALL_PREFIX}/include)
	link_directories(${CMAKE_INSTALL_PREFIX}/lib)
endif()

set(LIB_NAME "fruity")

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
