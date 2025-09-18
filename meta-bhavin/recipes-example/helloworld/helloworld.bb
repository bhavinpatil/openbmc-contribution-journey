SUMMARY = "Hello World Program"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://helloworld.c"

S = "${WORKDIR}/sources-unpack"

do_compile(){
    ${CC} ${CFLAGS} ${LDFLAGS} ${S}/helloworld.c -o ${S}/helloworld
}

do_install(){
    install -d ${D}${bindir}
    install -m 0755 ${S}/helloworld ${D}${bindir}/
}