# Copyright 2018-present Facebook. All Rights Reserved.
#
# This program file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program in a file named COPYING; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA

SUMMARY = "Miscellaneous Utilities Library"
DESCRIPTION = "Miscellaneous Utilities Library"

SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://misc-utils.h;beginline=4;endline=16;md5=da35978751a9d71b73679307c4d296ec"

SRC_URI = "file://Makefile \
           file://file-utils.c \
           file://path-utils.c \
           file://plat-utils.c \
           file://str-utils.c \
           file://misc-utils.h \
           "

S = "${WORKDIR}"

do_install() {
    install -d ${D}${libdir}
    install -m 0644 libmisc-utils.so ${D}${libdir}/libmisc-utils.so

    install -d ${D}${includedir}/openbmc
    install -m 0644 misc-utils.h ${D}${includedir}/openbmc/misc-utils.h
}

FILES_${PN} = "${libdir}/libmisc-utils.so"
FILES_${PN}-dev = "${includedir}/openbmc/misc-utils.h"
