do_install:append() {
    install -d ${D}${includedir}/dbus-1.0/dbus
    install -m 0644 ${B}/dbus/dbus-arch-deps.h ${D}${includedir}/dbus-1.0/dbus/
}
