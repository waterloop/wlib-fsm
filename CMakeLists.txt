cmake_minimum_required(VERSION 3.1.0)
project(wlib-fsm)

set(CMAKE_CXX_STANDARD 11)

set(WIO_DIR ${CMAKE_CURRENT_LIST_DIR}/.wio/node_modules)

set(malloc_dir ${WIO_DIR}/wlib-malloc)
set(memory_dir ${WIO_DIR}/wlib-memory)
set(tlsf_dir ${WIO_DIR}/wlib-tlsf)
set(tmp_dir ${WIO_DIR}/wlib-tmp)
set(fsm_dir ${CMAKE_CURRENT_LIST_DIR})

set(headers malloc memory tlsf tmp fsm)
foreach(header ${headers})
    set(${header}_include ${${header}_dir}/include)
endforeach()

set(modules malloc tlsf)
foreach(module ${modules})
    file(GLOB_RECURSE ${module}_files ${${module}_dir}/src/*.cpp)
    add_library(${module} ${${module}_files})
    target_include_directories(${module} PRIVATE ${${module}_include})
endforeach()

target_include_directories(malloc PRIVATE ${tlsf_include})
target_link_libraries(malloc tlsf)

add_executable(unit unit/main.cpp)
target_include_directories(unit PUBLIC ${fsm_include} ${tmp_include} ${memory_include} ${malloc_include})
target_link_libraries(unit malloc)

target_compile_definitions(tlsf PRIVATE
    WLIB_TLSF_64BIT
    WLIB_TLSF_LOG2_ALIGN=3
    WLIB_TLSF_LOG2_MAX=12
    WLIB_TLSF_LOG2_DIV=4
    WLIB_TLSF_DEBUG_LEVEL=1)
