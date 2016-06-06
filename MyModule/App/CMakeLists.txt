
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${OCC_INCLUDE_DIR}
    ${PYTHON_INCLUDE_PATH}
    ${ZLIB_INCLUDE_DIR}
    ${XERCESC_INCLUDE_DIR}
)

set(MyModule_LIBS
    FreeCADApp
)

SET(MyModule_SRCS
    AppMyModule.cpp
    AppMyModulePy.cpp
    PreCompiled.cpp
    PreCompiled.h
)

add_library(MyModule SHARED ${MyModule_SRCS})
target_link_libraries(MyModule ${MyModule_LIBS})


fc_target_copy_resource(MyModule 
    ${CMAKE_SOURCE_DIR}/src/Mod/MyModule
    ${CMAKE_BINARY_DIR}/Mod/MyModule
    Init.py)

SET_BIN_DIR(MyModule MyModule /Mod/MyModule)
SET_PYTHON_PREFIX_SUFFIX(MyModule)

install(TARGETS MyModule DESTINATION ${CMAKE_INSTALL_LIBDIR})
