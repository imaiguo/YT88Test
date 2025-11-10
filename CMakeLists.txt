
cmake_minimum_required(VERSION 3.31)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(APPNAME "YT88Test")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/devtools/Qt.6.9.1/lib/cmake")
# set(CMAKE_PREFIX_PATH "D:/devtools/Qt.6.9.1/static/lib/cmake")

project(${APPNAME} LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Sql REQUIRED)

message("current Qt version is[" ${QT_VERSION_MAJOR}  "]")

set(SRC
    src/Main.cpp
    src/DialogFunction.cpp
    src/sm3.cpp
    src/SoftkeyYT88.cpp
    src/dialog.ui
)

include_directories(src)

add_executable(${APPNAME} WIN32
    ${SRC}
)

target_link_libraries(${APPNAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Sql
        ${OPENSSL_LIBRARIES}
)

# file(COPY tmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/)
# file(COPY tmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/)
