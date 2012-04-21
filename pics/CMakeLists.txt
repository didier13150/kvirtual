set(KVIRTUAL_DATA_FILES
    tux.png
    centos.png
    debian.png
    fedora.png
    freebsd.png
    netbsd.png
    redhat.png
)

message(STATUS ${KDE4_DATA_DIR})
install( FILES ${KVIRTUAL_DATA_FILES} DESTINATION  ${KDE4_DATA_DIR}/kvirtual )
