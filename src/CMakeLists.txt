CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(QOSG)

SET(CMAKE_COLOR_MAKEFILE ON)

# All the Qt stuff
# http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

# set path to additional CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake ${CMAKE_MODULE_PATH})
find_package(Sphinx REQUIRED)

SET(QOSG_SOURCES main.cpp)
#SET(QOSQ_HEADERS )
#QT4_WRAP_CPP(QOSG_HEADERS_MOC ${QOSG_HEADERS})

ADD_EXECUTABLE(qosg ${QOSG_SOURCES})
TARGET_LINK_LIBRARIES(qosg ${QT_LIBRARIES})

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
