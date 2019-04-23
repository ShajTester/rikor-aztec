

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "file://linux-aspeed_dev-i2c.patch"
SRC_URI += "file://ftgmac100.patch"
# SRC_URI += "file://linux-aspeed_dev-eth.patch"
SRC_URI += "file://ast-video.patch"

