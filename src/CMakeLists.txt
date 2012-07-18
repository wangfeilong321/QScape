PROJECT(QOSG)

# CMake Setup
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake ${CMAKE_MODULE_PATH})

# All the Qt stuff
# http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

# QOSG Source Files
SET(QOSG_SOURCES
    main.cpp
    mainwindow.cpp)
SET(QOSG_HEADERS
    mainwindow.h)
SET(QOSG_FORMS
    ui/mainwindow.ui)

# QOSG Build Instructions
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
QT4_WRAP_CPP(QOSG_HEADERS_MOC ${QOSG_HEADERS})
QT4_WRAP_UI(QOSG_FORMS_HEADERS ${QOSG_FORMS})
ADD_EXECUTABLE(qosg ${QOSG_SOURCES}
    ${QOSG_HEADERS_MOC}
    ${QOSG_FORMS_HEADERS})
TARGET_LINK_LIBRARIES(qosg ${QT_LIBRARIES})

# QOSG Documentation Build Instructions
find_package(Sphinx REQUIRED)
# configured documentation tools and intermediate build results
set(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")
# Sphinx cache with pickled ReST documents
set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_doctrees")
# HTML output directory
set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/html")

add_custom_target(docs ALL
    ${SPHINX_EXECUTABLE}
    -q -b html
    -d "${SPHINX_CACHE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../doc"
    "${SPHINX_HTML_DIR}"
    COMMENT "Building HTML documentation with Sphinx")

