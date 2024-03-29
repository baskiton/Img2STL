cmake_minimum_required(VERSION 3.19)
project(Img2STL VERSION 2.0.2)

set(CMAKE_CXX_STANDARD 17)


if(WIN32)
        set(wxWidgets_ROOT_DIR "dep/win/")
        add_executable(${PROJECT_NAME} WIN32 res/res_msw.rc)
        find_package(wxWidgets REQUIRED mono)

        target_link_options(
                ${PROJECT_NAME} PRIVATE
                -static -lpthread
        )

elseif(MACOSX_BUNDLE)
        add_executable(${PROJECT_NAME} MACOSX_BUNDLE)

else()  # Linux
        set(wxWidgets_CONFIG_OPTIONS --static=yes --version=3.1)
        add_executable(${PROJECT_NAME})

        find_package(wxWidgets REQUIRED std aui propgrid)

endif()

include(${wxWidgets_USE_FILE})
include_directories(
        include
        res
        ${wxWidgets_INCLUDE_DIRS}
)

target_link_libraries(
        ${PROJECT_NAME}
        ${wxWidgets_LIBRARIES}
)

target_sources(
        ${PROJECT_NAME}
        PUBLIC

        src/Img2STL.cpp
        src/Img2STLMainFrame.cpp
        src/Img2STLAboutDialog.cpp
        src/ui/ui.cpp
        src/Executor.cpp
        src/Worker.cpp
        src/ImgExtFunc.cpp
        src/STLFile.cpp
        src/Logging.cpp
)

target_compile_options(
        ${PROJECT_NAME} PRIVATE
        -Wall
)

target_link_options(
        ${PROJECT_NAME} PRIVATE
        -static-libgcc
        -static-libstdc++
        -Wall
)

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
