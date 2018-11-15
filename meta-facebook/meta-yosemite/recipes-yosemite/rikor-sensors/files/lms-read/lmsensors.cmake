
# Отсюда
# http://qaru.site/questions/955697/finding-a-directory-in-cmake

# - Try to find the LM_SENSORS library.
#
# The following are set after configuration is done: 
#  LM_SENSORS_FOUND
#  LM_SENSORS_INCLUDE_DIRS
#  LM_SENSORS_LIBRARY_DIRS
#  LM_SENSORS_LIBRARIES

find_path(LM_SENSORS_INCLUDE_DIR NAMES sensors/sensors.h)
find_library(LM_SENSORS_LIBRARY NAMES libsensors sensors)

message("LM_SENSORS include dir = ${LM_SENSORS_INCLUDE_DIR}")
message("LM_SENSORS lib = ${LM_SENSORS_LIBRARY}")

set(LM_SENSORS_LIBRARIES ${LM_SENSORS_LIBRARY})
set(LM_SENSORS_INCLUDE_DIRS ${LM_SENSORS_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# Handle the QUIETLY and REQUIRED arguments and set the LM_SENSORS_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LM_SENSORS DEFAULT_MSG
                                  LM_SENSORS_LIBRARY LM_SENSORS_INCLUDE_DIR)

mark_as_advanced(LM_SENSORS_INCLUDE_DIR LM_SENSORS_LIBRARY)

