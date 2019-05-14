# Copyright 2018-present Facebook. All Rights Reserved.

inherit aspeed_uboot_image

require recipes-core/images/fb-openbmc-image.bb

# Changing the image compression from gz to lzma achieves 30% saving (~3M).
IMAGE_FSTYPES += "cpio.lzma.u-boot"
UBOOT_IMAGE_ENTRYPOINT = "0x40008000"

# Include modules in rootfs
IMAGE_INSTALL += " \
  packagegroup-openbmc-base \
  packagegroup-openbmc-net \
  openbmc-utils \
  sensor-setup \
  rikgpio \
  lighttpd \
  rikcgi \
  rikfans \
  i2c-tools \
  fan-ctrl \
  rikor-fru \
  dscan \
  rikkvm \
  riktest \
  "
#  riknet 
#  rikkvm

#  packagegroup-openbmc-python3y 

IMAGE_INSTALL += "rikcgi-login"

IMAGE_INSTALL += "watchdog-ctrl"


IMAGE_INSTALL += "ledblink-lt"
IMAGE_INSTALL += "initsc-lt"
IMAGE_INSTALL += "rikbtnd"


  
IMAGE_FEATURES += " \
  "

DISTRO_FEATURES += " \
  "
