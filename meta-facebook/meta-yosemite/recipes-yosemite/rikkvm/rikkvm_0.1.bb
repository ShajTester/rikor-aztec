# Copyright 2014-present Facebook. All Rights Reserved.
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
#

SUMMARY = "Terminal Multiplexer"
DESCRIPTION = "Util for multiplexing terminal"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://Makefile;md5=2e279cb93a43885e0feb3c14e8f7b84a"

SRC_URI = "file://kvm_main.cpp \
	   file://ikvm_args.cpp \
	   file://ikvm_args.hpp \
           file://ikvm_manager.cpp \
           file://ikvm_manager.hpp \
           file://Makefile \
          "

S = "${WORKDIR}"

pkgdir = "rikkvm"

do_install() {
  dst="${D}/usr/local/fbpackages/${pkgdir}"
  bin="${D}/usr/local/bin"
  install -d $dst
  install -d $bin
  install -m 755 rikkvm ${dst}/rikkvm
  ln -snf ../fbpackages/${pkgdir}/rikkvm ${bin}/rikkvm
}

FBPACKAGEDIR = "${prefix}/local/fbpackages"

FILES_${PN} = "${FBPACKAGEDIR}/rikkvm ${prefix}/local/bin"

