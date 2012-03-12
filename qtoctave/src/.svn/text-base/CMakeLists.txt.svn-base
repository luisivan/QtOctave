FIND_PACKAGE(Qt4 REQUIRED)

MESSAGE (STATUS "Use file: ${QT_USE_FILE}")

SET(QT_USE_QTXML 1)

include(UseQt4)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR})
include_directories(${QT_QTGUI_INCLUDE_DIR})
include_directories(${QT_QTXML_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${QT_QTSVG_INCLUDE_DIR})

FILE(GLOB_RECURSE qtoctave_moc_headers *.h  )
FILE(GLOB_RECURSE qtoctave_sources *.cpp )
FILE(GLOB_RECURSE qtoctave_ui *.ui )

configure_file( configure.h.in ${CMAKE_CURRENT_BINARY_DIR}/configure.h )

# run uic on .ui files:
QT4_WRAP_UI( qtoctave_ui_headers ${qtoctave_ui} )

# run moc on these files:
QT4_WRAP_CPP( qtoctave_moc_sources ${qtoctave_moc_headers} )

# Generate qm files if needed
file(GLOB lang_files "lang/*.ts" GLOB "lang/*.qph")
foreach(lang_file ${lang_files})
  get_filename_component(this_qm ${lang_file} NAME_WE)
  set(this_qm "${this_qm}.qm")
  add_custom_command(OUTPUT ${this_qm}
    COMMAND "lrelease"
    ARGS ${lang_file} -qm "${this_qm}"
    DEPENDS ${lang_file}
    )
  set(qm "${qm};${this_qm}")
  set(qm_build "${qm_build};${CMAKE_CURRENT_BINARY_DIR}/${this_qm}")
endforeach()


add_executable(qtoctave
	${qtoctave_sources} ${qtoctave_moc_sources} ${qtoctave_ui_headers} ${qtoctave_moc_headers} ${qm}
)

SET_TARGET_PROPERTIES(qtoctave
	PROPERTIES INSTALL_RPATH ":usr/lib"
	BUILD_WITH_INSTALL_RPATH true
	INSTALL_RPATH_USE_LINK_PATH true
)

TARGET_LINK_LIBRARIES(qtoctave ${Qt4_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTSVG_LIBRARY})

install (
    DIRECTORY images menus scripts scripts_octave syntax_files fonts
	DESTINATION share/qtoctave
	PATTERN ".svn" EXCLUDE
        )

install(
  FILES ${qm_build}
  DESTINATION share/qtoctave/lang
)



install (
	DIRECTORY octave_doc
	DESTINATION share/doc/octave-html
	PATTERN ".svn" EXCLUDE
	)

install (
	DIRECTORY qtoctave_doc
	DESTINATION share/doc/qtoctave
	PATTERN ".svn" EXCLUDE
	)

install	(
	TARGETS qtoctave
	RUNTIME DESTINATION bin
	)

install (
	FILES config_files/config.rc
	DESTINATION share/qtoctave
	) 
 
install (
               FILES config_files/qtoctave.desktop
               DESTINATION share/applications
       )

install (
               FILES config_files/qtoctave.png
               DESTINATION share/icons/hicolor/64x64/apps/
        )
