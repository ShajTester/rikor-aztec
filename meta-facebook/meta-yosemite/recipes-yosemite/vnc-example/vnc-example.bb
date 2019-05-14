


SUMMARY = "KVM"
DESCRIPTION = "KVM"
SECTION = "example"
PR = "r1"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=cc4734e8a01f740811ac41130a40cf90"

DEPENDS += "libvncserver glibc"
# RDEPENDS_${PN} += ""

SRCREV = "${AUTOREV}"
SRC_URI = "file://."

inherit cmake
EXTRA_OECMAKE += "-DCMAKE_BUILD_TYPE=MinSizeRel -DBUILD_SHARED_LIBS=ON"


S = "${WORKDIR}"


