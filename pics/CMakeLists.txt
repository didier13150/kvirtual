set(KVIRTUAL_DATA_FILES
    linux.png
    centos.png
    debian.png
    fedora.png
    freebsd.png
    netbsd.png
    redhat.png
    mandriva.png
)

message(STATUS ${KDE4_DATA_DIR})
install( FILES ${KVIRTUAL_DATA_FILES} DESTINATION  ${KDE4_DATA_DIR}/kvirtual )
