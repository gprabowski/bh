set(SOURCES
    main.cpp
    gui.cpp
    callbacks.cpp
    init.cpp
    input_handlers.cpp
    log.cpp
    frame_state.cpp
    utils.cpp	
    framebuffer.cpp
    frame_render.cpp
    frame_update.cpp
    shader_manager.cpp
    scene.cpp
    gl_object.cpp
    cube.cpp
    stb_image.cpp
)

add_executable(czarnadziura)
find_program( CLANG_TIDY_EXE NAMES "clang-tidy" DOC "Path to clang-tidy executable" ) 
if(NOT CLANG_TIDY_EXE) 
    message(STATUS "clang-tidy not found.") 
else() 
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}") 
    message(STATUS "clang-tidy turned off")
    #set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-clang-analyzer-alpha.*") 
endif()
set_target_properties( czarnadziura PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON )
if(CLANG_TIDY_EXE) 
    set_target_properties( czarnadziura PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}" ) 
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(czarnadziura PUBLIC -Werror -Wall)
endif()
target_compile_features(czarnadziura PUBLIC cxx_std_17)
set_target_properties(czarnadziura PROPERTIES
    CXX_STANDARD 17 )

target_sources(czarnadziura PUBLIC ${SOURCES})

message("CMAKE ROOT IS ${CMAKE_SOURCE_DIR}")
target_include_directories(czarnadziura 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/thirdparty/glm/glm
    ${CMAKE_SOURCE_DIR}/thirdparty/mtxlib
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}/thirdparty/implot
    ${CMAKE_SOURCE_DIR}/thirdparty/glfw/include/
    ${CMAKE_SOURCE_DIR}/thirdparty/spdlog/include/
    ${CMAKE_SOURCE_DIR}/include
 )

message(STATUS "<<${OPENGL_INCLUDE_DIRS}>>")

target_link_libraries(czarnadziura 
        glad
        mtxlib
        spdlog
        glfw
        glm
        imgui
        implot
)

add_custom_command(TARGET czarnadziura PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E create_symlink
                       ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:czarnadziura>/resources)

add_custom_command(TARGET czarnadziura PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E create_symlink
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:czarnadziura>/assets)

add_custom_command(TARGET czarnadziura PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E create_symlink
                       ${CMAKE_SOURCE_DIR}/fonts/ $<TARGET_FILE_DIR:czarnadziura>/fonts)
