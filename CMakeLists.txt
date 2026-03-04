cmake_minimum_required(VERSION 3.11)

project(Project_Crystallis)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    pspgu
    pspge
    pspdisplay
    pspdebug
    pspctrl
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    libraries
)

# Create an EBOOT.PBP file
create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE ${PROJECT_NAME}
    VERSION 01.00
)



# # Define where your assets are
# set(ASSET_FILES
#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/fffont.fnt
#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/fffont_0.png
#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/gamefont.fnt
#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/gamefont_0.png
# )

# # Copy assets to the build directory whenever they change
# foreach(ASSET ${ASSET_FILES})
#     get_filename_component(ASSET_NAME ${ASSET} NAME)
#     add_custom_command(
#         TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         ${ASSET}
#         $<TARGET_FILE_DIR:${PROJECT_NAME}>/${ASSET_NAME}
#     )
# endforeach()