cmake_minimum_required(VERSION 3.26.0)
project(converter_ascii VERSION 0.1.0)

set(wxWidgets_ROOT_DIR "C:/wxWidgets")
set(wxWidgets_LIB_DIR "C:/wxWidgets/lib/gcc810_x64_dll")

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

file(GLOB project_GLOB
    ./*
)

add_executable(converter_ascii ${project_GLOB})

target_link_libraries(converter_ascii ${wxWidgets_LIBRARIES})
target_include_directories(converter_ascii PRIVATE ${wxWidgets_INCLUDE_DIRS}) 