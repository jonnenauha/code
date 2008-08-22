INCLUDE (CheckLibraryExists)
INCLUDE (CheckIncludeFiles)

PROJECT (voiceforvw)

SET (CMAKE_BUILD_TYPE Debug)

# included source variables
SET (VOICEDIR ${CMAKE_SOURCE_DIR}/.)
SET (PJDIR ${CMAKE_SOURCE_DIR}/pjproject)
SET (SOCKETDIR ${CMAKE_SOURCE_DIR}/sockets)
SET (TINYXMLDIR ${CMAKE_SOURCE_DIR}/tinyxml)

SET (VOICESRC 
    ${VOICEDIR}/main.cpp
    ${VOICEDIR}/sip.cpp
    ${VOICEDIR}/server.cpp
    ${VOICEDIR}/state.cpp
    ${VOICEDIR}/parsing.cpp
    ${VOICEDIR}/parameters.cpp
    ${VOICEDIR}/base64.cpp)

SET (SOCKETSRC 
    ${SOCKETDIR}/Sockets.cpp)

SET (TINYXMLSRC 
    ${TINYXMLDIR}/tinyxml.cpp 
    ${TINYXMLDIR}/tinyxmlerror.cpp
    ${TINYXMLDIR}/tinyxmlparser.cpp 
    ${TINYXMLDIR}/tinystr.cpp )
    
# where to find PJSIP files locally
FILE (GLOB PJINCLDIR ${PJDIR}/*/include)
FILE (GLOB PJLIBDIR ${PJDIR}/*/lib)

# cause tinyxml to use the STL
ADD_DEFINITIONS (-DTIXML_USE_STL)

# platform specific area

# Windows
IF (WIN32)
    ADD_DEFINITIONS(-DPJ_WIN32=1)
    
    # necessary system-level libraries
    SET (PJTARGET i386-win32-vc8-release)
    SET (LIBS ${LIBS} wsock32.lib ws2_32.lib)
ENDIF (WIN32)

# Unix-likes
IF (UNIX)

    # MacOS
    IF (APPLE)
        ADD_DEFINITIONS (-DPJ_AUTOCONF=0)
        SET (PJTARGET i386-apple-darwin8.11.1)
    ELSE (APPLE)

    # Linux
        ADD_DEFINITIONS (-DPJ_AUTOCONF=1)
        SET (PJTARGET i686-pc-linux-gnu)
    ENDIF (APPLE)

    # necessary system-level libraries
    SET (LIBS ${LIBS} m uuid nsl rt pthread asound ssl crypto)

    # FIND_LIBRARY unfortunately prefers dynamic over static
    # with no option to override. We break out the sledgehammer.
    #FOREACH (L ${LIBS})
    #    STRING (REPLACE ".so" ".a" SL ${L})
    #    SET (STATIC_LIBS ${STATIC_LIBS} ${SL})
    #ENDFOREACH (L)
ENDIF (UNIX)
    
# PJSIP is inconsistent in naming across platforms
MACRO (FIND_PJSIP_LIBRARY N P)
    FIND_LIBRARY (${N}_LIB 
        NAMES 
        ${N} 
        "${N}-${PJTARGET}" 
        "lib${N}" 
        "lib${N}-${PJTARGET}"
        "${N}-lib"
        "${N}-lib-${PJTARGET}"
        "${N}lib"
        "${N}lib-${PJTARGET}"
        "${N}-core"
        "${N}-core-${PJTARGET}"
        PATHS ${PJDIR}/${P}/lib)
    SET (PJLIBS ${PJLIBS} ${${N}_LIB})
ENDMACRO (FIND_PJSIP_LIBRARY)

# included PJSIP libraries
FIND_PJSIP_LIBRARY (pjsua pjsip)
FIND_PJSIP_LIBRARY (pjsip-ua pjsip)
FIND_PJSIP_LIBRARY (pjsip-simple pjsip)
FIND_PJSIP_LIBRARY (pjsip pjsip)
FIND_PJSIP_LIBRARY (pjmedia-codec pjmedia)
FIND_PJSIP_LIBRARY (pjmedia pjmedia)
FIND_PJSIP_LIBRARY (pjnath pjnath)
FIND_PJSIP_LIBRARY (pjlib-util pjlib-util)
FIND_PJSIP_LIBRARY (pj pjlib)
FIND_PJSIP_LIBRARY (resample third_party)
FIND_PJSIP_LIBRARY (milenage third_party)
FIND_PJSIP_LIBRARY (gsmcodec third_party)
FIND_PJSIP_LIBRARY (ilbccodec third_party)
FIND_PJSIP_LIBRARY (portaudio third_party)
FIND_PJSIP_LIBRARY (speex third_party)

# find headers
FIND_FILE (PJINCL 
    NAMES "pjsua-lib/pjsua.h" 
    PATHS "${PJDIR}/pjsip/include" ENV INCLUDE)

# aggregate the variables
SET (SRC ${VOICESRC} ${SOCKETSRC} ${TINYXMLSRC})
SET (LIBS ${LIBS} ${PJLIBS})

SET (INCLDIR ${VOICEDIR} ${PJINCLDIR})
SET (LIBDIR ${PJLIBDIR})

# set the build
INCLUDE_DIRECTORIES (${INCLDIR})
LINK_DIRECTORIES (${LIBDIR})

ADD_EXECUTABLE (SLVoice ${SRC})
TARGET_LINK_LIBRARIES (SLVoice ${LIBS})
