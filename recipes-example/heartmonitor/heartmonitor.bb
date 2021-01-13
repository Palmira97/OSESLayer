DESCRIPTION = "Heart monitor User App that reads data from a ppg sensor"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://heartmonitor.c"

S = "${WORKDIR}"

do_compile() {
	set CFLAGS -g
	${CC} ${CFLAGS} -lm heartmonitor.c ${LDFLAGS} -o heartmonitor
	unset CFLAGS
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 heartmonitor ${D}${bindir}
}
