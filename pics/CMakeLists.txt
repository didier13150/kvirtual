set(KVIRTUAL_DATA_FILES
    linux.png
    centos.png
    debian.png
    fedora.png
    freebsd.png
    netbsd.png
    redhat.png
    mandriva.png
    openbsd.png
    windows.png
    archlinux.png
    gentoo.png
    knoppix.png
    linuxmint.png
    mepis.png
    opensuse.png
    slackware.png
    ubuntu.png
)

message(STATUS ${KDE4_DATA_DIR})
install( FILES ${KVIRTUAL_DATA_FILES} DESTINATION  ${KDE4_DATA_DIR}/kvirtual )
