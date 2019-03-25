


SUMMARY = "CGI scripts"
DESCRIPTION = "Login CGI scripts"
SECTION = "base"
PR = "r1"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=cc4734e8a01f740811ac41130a40cf90"

SRCREV = "${AUTOREV}"
SRC_URI = "file://login "

DEPEND = " glibc "
RDEPEND_${PN} = " glibc "

S = "${WORKDIR}/login"

# export SINC = "${STAGING_INCDIR}"
# export SLIB = "${STAGING_LIBDIR}"

inherit cmake
EXTRA_OECMAKE += " -DCMAKE_BUILD_TYPE=MinSizeRel -DBUILD_SHARED_LIBS=ON "

# do_install_append() {
#   dst="${D}/www/pages"
#   bin="${D}/usr/bin"
#   install -d $dst
#   install -d $bin
#   ln -snf /usr/bin/rikcgi-login ${dst}/login.sh 
# }

# FILES_${PN} += " /www/pages/login.sh "

BBCLASSEXTEND = "native"

# The autotools configuration I am basing this on seems to have a problem with a race condition when parallel make is enabled
PARALLEL_MAKE = ""

FBPACKAGEDIR = "/www/pages"


