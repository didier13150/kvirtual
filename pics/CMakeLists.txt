set(KVIRTUAL_DATA_FILES
    linux.svg
    centos.svg
    debian.svg
    fedora.svg
    freebsd.svg
    netbsd.svg
    redhat.svg
    mandriva.svg
    openbsd.svg
    windows.svg
    archlinux.svg
    gentoo.svg
    knoppix.svg
    linuxmint.svg
    mepis.svg
    opensuse.svg
    slackware.svg
    ubuntu.svg
)

message(STATUS ${KDE4_DATA_DIR})
install( FILES ${KVIRTUAL_DATA_FILES} DESTINATION  ${KDE4_DATA_DIR}/kvirtual )
