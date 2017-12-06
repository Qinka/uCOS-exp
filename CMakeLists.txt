# cmake for uC/OS II exp 

## require for CMake
cmake_minimum_required(VERSION 3.3)

## build system set up
set (CMAKE_SYSTEM_NAME      Generic)
set (CMAKE_SYSTEM_PROCESSOR arm)
set (CMAKE_CROSSCOMPILING   TRUE)

## project
project(ucos-exp)

## languagse
enable_language(C ASM)

## config the ucos
set(PATH_CONFIG "cmake")
include("${PATH_CONFIG}/config.cmake")

## for platform
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
  if (NOT (DEFINED "EXE_IGNORE"))
    set (EXE_POSTFIX ".exe") 
  endif (NOT (DEFINED "EXE_IGNORE"))
endif (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")


## add include path
include_directories (include)
include_directories (${PROJECT_SOURCE_DIR}/ucos-ii-stm32f103ze/include)

link_directories(${PROJECT_SOURCE_DIR}/ucos-ii-stm32f103ze/build/lib)
set (CMAKE_EXE_LINKER_FLAGS " -T ${PROJECT_SOURCE_DIR}/ucos-ii-stm32f103ze/ucos.ld")

## task manage
add_executable(task_manage source/hardware.c source/task_manage.c)
target_link_libraries(task_manage ucos)