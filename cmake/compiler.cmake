## cross compiler prefix
set (ARM_PREFIX "arm-unknown-eabi-" CACHE STRING "The cross compiler prefix")


## compiler macro
## debug
option(CONFIG_DEBUG "Turn on debug" ON)
if (${CONFIG_DEBUG})
  set(INNER_CONFIG_BUG " -g ")
else (${CONFIG_DEBUG})
  set(INNER_CONFIG_BUG " ")
endif (${CONFIG_DEBUG})
## device type
set (CONFIG_DEV "STM32F10X_HD" CACHE string "the kind of device")
## config compiling
## set up compiler
set (CMAKE_C_COMPILER   "${ARM_PREFIX}gcc")
set (CMAKE_CXX_COMPILER "${ARM_PREFIX}g++")
set (CMAKE_C_FLAGS      "$ENV{CCFLAGS} ${INNER_CONFIG_BUG}  -mcpu=cortex-m3 -mthumb -nostdlib")
set (CMAKE_ASM_FLAGS      "$ENV{CCFLAGS} ${INNER_CONFIG_BUG}  -mcpu=cortex-m3 -mthumb -nostdlib")
add_definitions("-D${CONFIG_DEV}")
set (CMAKE_ASM_COMPILER "${ARM_PREFIX}gcc")
set (CMAKE_AR "${ARM_PREFIX}ar")
set (CMAKE_RANLIB "${ARM_PREFIX}ranlib")