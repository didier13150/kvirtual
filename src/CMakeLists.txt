set(kvirtual_SRCS
   kvirtualoptions.cpp
   kvirtualprocess.cpp
   kvirtual.cpp
   main.cpp
   kvirtualview.cpp
 )

kde4_add_ui_files(kvirtual_SRCS kvirtualview_base.ui prefs_base.ui)

kde4_add_kcfg_files(kvirtual_SRCS settings.kcfgc )

kde4_add_executable(kvirtual ${kvirtual_SRCS})

target_link_libraries(kvirtual ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS kvirtual ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS kvirtual.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kvirtual.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kvirtualui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kvirtual )
