cmake_minimum_required(VERSION 2.8.0)

# .deb packaging
set(ARCH "i686")
if(${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(ARCH "x86_64")
endif ()

set(DEFAULT_ETC_ALTERNATIVES_PRIORITY 2)

# The format of the description field is a short summary line followed by a
# longer paragraph indented by a single space on each line
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "library/utility for creating process streams on Linux (pipes, fanout)")
set(CPACK_PACKAGE_NAME "streamgraph${EXE_VERSION_SUFFIX}")
set(CPACK_PACKAGE_VENDOR "wugc")
set(CPACK_PACKAGE_VERSION ${FULL_VERSION})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Allison Regier <aregier@genome.wustl.edu>")
set(CPACK_SYSTEM_NAME "Linux-${ARCH}")
set(CPACK_TOPLEVEL_TAG "Linux-${ARCH}")
set(CPACK_DEBIAN_PACKAGE_PROVIDES "streamgraph")
set(CPACK_DEBIAN_PACKAGE_SECTION science)
set(CPACK_DEBIAN_PACKAGE_PRIORITY optional)
set(CPACK_DEBIAN_PACKAGE_REPLACES "streamgraph")
set(CPACK_DEBIAN_PACKAGE_CONFLICTS "streamgraph")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.11.1-0ubuntu7.8), libgcc1 (>= 1:4.4.3-4ubuntu5), libstdc++6 (>= 4.4.3-4ubuntu5)")

if (CMAKE_BUILD_TYPE MATCHES package)
    set(CPACK_GENERATOR "DEB")
else(CMAKE_BUILD_TYPE MATCHES package)
    set(CPACK_GENERATOR "TGZ")
endif(CMAKE_BUILD_TYPE MATCHES package)

include(CPack)
