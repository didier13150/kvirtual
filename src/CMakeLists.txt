set(kvirtual_SRCS 
   kvirtualsettingsview.cpp
   kvirtualiface.cpp
   kvirtualstorage.cpp
   kvirtualdevice.cpp
   kvirtualcreateimg.cpp
   kvirtualoptions.cpp
   kvirtualprocess.cpp
   kvirtual.cpp
   main.cpp
   kvirtualview.cpp
   kvirtualstoragewidget.cpp
   kvirtualifacewidget.cpp
)

kde4_add_ui_files(kvirtual_SRCS
   kvirtualiface_widget.ui
   kvirtualstorage_widget.ui
   kvirtualview_base.ui
   prefs_base.ui
   kvirtual_create_img.ui
)

kde4_add_kcfg_files(kvirtual_SRCS settings.kcfgc )

kde4_add_executable(kvirtual ${kvirtual_SRCS})

target_link_libraries(kvirtual ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS kvirtual ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS kvirtual.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kvirtual.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kvirtualui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kvirtual )
install( FILES kvirtual.notifyrc  DESTINATION  ${DATA_INSTALL_DIR}/kvirtual )
