PROJECT(QSCAPE)

# CMake Setup
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake ${CMAKE_MODULE_PATH})

FIND_PACKAGE(OpenSceneGraph REQUIRED)

# All the Qt stuff
# http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

# QScape Source Files
SET(QSCAPE_SOURCES
    main.cpp
    mainwindow.cpp
    viewerwidget.cpp)
SET(QSCAPE_HEADERS
    mainwindow.h
    viewerwidget.h)
SET(QSCAPE_FORMS
    ui/mainwindow.ui)

# QScape Build Instructions
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${OPENSCENEGRAPH_INCLUDE_DIRS})
QT4_WRAP_CPP(QSCAPE_HEADERS_MOC ${QSCAPE_HEADERS})
QT4_WRAP_UI(QSCAPE_FORMS_HEADERS ${QSCAPE_FORMS})
ADD_EXECUTABLE(qscape ${QSCAPE_SOURCES}
    ${QSCAPE_HEADERS_MOC}
    ${QSCAPE_FORMS_HEADERS})
TARGET_LINK_LIBRARIES(qscape
    ${QT_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
    osgAnimation
    osgGA
    osgDB
    osgUtil
    osgWidget
    osgViewer
    osgQt)

# QScape Documentation Build Instructions
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

