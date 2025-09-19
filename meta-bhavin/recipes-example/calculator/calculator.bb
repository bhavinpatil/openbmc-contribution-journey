SUMMARY = "Simple Calculator Application Package"
DESCRIPTION = "A simple calculator application that performs basic arithmetic operations using shared libraries."
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://main.c \
           file://calc.h \
           file://libadd.c \
           file://libsubtract.c \
           file://libmultiply.c \
           file://libdivide.c \
           file://Makefile \
          "

S = "${WORKDIR}/sources-unpack"

LIBS = "libadd.so libsubtract.so libmultiply.so libdivide.so"

# Pass CC, CFLAGS, LDFLAGS to Makefile
EXTRA_OEMAKE = "CC='${CC}' CFLAGS='${CFLAGS}' LDFLAGS='${LDFLAGS}'"
EXTRA_OEMAKE:append = " DESTDIR=${D} BINDIR=${bindir} LIBDIR=${libdir}"
EXTRA_OEMAKE:append = " TARGET=${PN} LIBS='${LIBS}'"

do_compile () {
    oe_runmake -C ${S}
}

do_install () {
    oe_runmake -C ${S} install
}
