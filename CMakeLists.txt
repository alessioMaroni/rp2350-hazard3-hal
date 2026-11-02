# Minimum CMake version required to configure and build this project
cmake_minimum_required(VERSION 4.4.4)

# Configure cross-compilation target environment
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv32)

# Define the project name, version, description, and supported languages
project(rp2350-hazard3-hal
    VERSION 0.1.2
    DESCRIPTION "Bare-metal MISRA C compliant HAL for RP2350 Hazard3 RISC-V"
    LANGUAGES C
)

# Enforce C99 standard strictly without compiler-specific extensions
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Define strict compile-time flags and target architecture settings (RISC-V 32-bit)
set(SAFETY_COMPILE_OPTIONS
    "-Wall" "-Wextra" "-Werror" "-Wpedantic"
    "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wshadow"
    "-Wpointer-arith" "-Wcast-qual" "-Wwrite-strings" "-Wconversion"
    "-fno-strict-aliasing"
    "-ffreestanding"
    "-march=rv32imac_zicsr_zifencei_zba_zbb_zbkb_zbs"
    "-mabi=ilp32"
    "-mcmodel=medlow"
)

# Configure optimization and debug flags depending on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND SAFETY_COMPILE_OPTIONS "-O2")
else()
    list(APPEND SAFETY_COMPILE_OPTIONS "-O0" "-g")
endif()

# Define the static library target with its source files
add_library(rp2350-hazard3-hal STATIC
    src/serial.c
    src/time.c
)

# Apply compilation flags specifically for GCC and Clang compilers
target_compile_options(rp2350-hazard3-hal PRIVATE
    $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:${SAFETY_COMPILE_OPTIONS}>
)

# Set the public include directories for the library target
target_include_directories(rp2350-hazard3-hal PUBLIC include)
