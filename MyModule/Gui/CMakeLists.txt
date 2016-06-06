
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${COIN3D_INCLUDE_DIR}
    ${QT_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ${PYTHON_INCLUDE_PATH}
    ${XERCESC_INCLUDE_DIR}
)

set(MyModuleGui_LIBS
    MyModule
    FreeCADGui
)

qt4_add_resources(MyModule_QRC_SRCS Resources/MyModule.qrc)

SET(MyModuleGui_SRCS
    ${MyModule_QRC_SRCS}
    AppMyModuleGui.cpp
    AppMyModuleGuiPy.cpp
    Command.cpp
    PreCompiled.cpp
    PreCompiled.h
    Workbench.cpp
    Workbench.h
)

add_library(MyModuleGui SHARED ${MyModuleGui_SRCS})
target_link_libraries(MyModuleGui ${MyModuleGui_LIBS})

fc_target_copy_resource(MyModuleGui 
    ${CMAKE_SOURCE_DIR}/src/Mod/MyModule
    ${CMAKE_BINARY_DIR}/Mod/MyModule
    InitGui.py)

SET_BIN_DIR(MyModuleGui MyModuleGui /Mod/MyModule)
SET_PYTHON_PREFIX_SUFFIX(MyModuleGui)
    
install(TARGETS MyModuleGui DESTINATION ${CMAKE_INSTALL_LIBDIR})
