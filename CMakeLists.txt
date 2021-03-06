add_library(llenc SHARED llenc.cpp)
    
if(OMEGA_OS_WIN)
    if(OMEGA_ARCH_32)
        target_link_libraries(llenc ${CMAKE_CURRENT_LIST_DIR}/GRID-2.3/x86/NvIFROpenGL.lib)
    else()
        target_link_libraries(llenc ${CMAKE_CURRENT_LIST_DIR}/GRID-2.3/amd64/NvIFROpenGL.lib)
    endif()
elseif(OMEGA_OS_LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lnvidia-ifr")
else()
    message(FATAL "Nvidia GRID SDK not supported on this OS.")
endif()
    
set_target_properties(llenc PROPERTIES FOLDER modules)
target_link_libraries(llenc omegaToolkit)

if(OMEGA_BUILD_EXAMPLES)
    add_executable(helloEncoder
        examples/cube.cpp
        examples/helloEncoder.cpp)
    set_target_properties(helloEncoder PROPERTIES FOLDER examples)
    target_link_libraries(helloEncoder omega llenc)
endif()