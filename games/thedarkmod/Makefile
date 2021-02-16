# $OpenBSD: Makefile,v 1.2 2019/09/03 16:56:43 sthen Exp $

ONLY_FOR_ARCHS =	amd64 i386
BROKEN-i386 =	Simd_SSE.cpp fails, unknown type name '__m128'

COMMENT =	first-person stealth game inspired by Thief

V =		2.09
DISTNAME =	thedarkmod.${V}
PKGNAME =	thedarkmod-${V}

CATEGORIES =	games x11
HOMEPAGE =	http://www.thedarkmod.com
MAINTAINER =	Thomas Frohwein <thfr@openbsd.org>

# GPLv3/revised BSD (engine), CC-BY-NC-SA 3.0 (data)
PERMIT_PACKAGE =	Yes

WANTLIB += ${COMPILER_LIBCXX} IL X11 Xau Xdmcp Xext Xxf86vm
WANTLIB += avcodec avdevice avfilter avformat avresample avutil
WANTLIB += c curl execinfo jpeg m minizip openal pugixml quazip5
WANTLIB += swresample swscale vorbisfile xcb z

MASTER_SITES =		http://www.thedarkmod.com/sources/
DISTFILES =		${DISTNAME}.src.7z
EXTRACT_CASES =	*.7z) ${LOCALBASE}/bin/7z x -o${WRKSRC} ${DISTDIR}/$$archive > \
			/dev/null;;

# C++14 as of CMakeList, but maybe uses C++17 stuff (stdc++fs)
COMPILER =	base-clang ports-gcc

# need to add this for gcc
# revisit when gcc drops it
EXTRA_ports-gcc =	-lstdc++fs
LDFLAGS +=		${EXTRA_${CHOSEN_COMPILER}}

MODULES =		devel/cmake
BUILD_DEPENDS =		archivers/p7zip
LIB_DEPENDS =		archivers/minizip \
			archivers/quazip \
			audio/openal \
			graphics/DevIL \
			graphics/ffmpeg \
			net/curl \
			textproc/pugixml

# CONFIGURE_ARGS +=	-DCMAKE_CFLAGS="-I${X11BASE}/include"

CFLAGS +=		-I${X11BASE}/include -I${LOCALBASE}/include
CXXFLAGS +=		-I${X11BASE}/include -I${LOCALBASE}/include
CONFIGURE_ENV +=	LDFLAGS="-L${X11BASE}/lib -L${LOCALBASE}/lib"
CONFIGURE_ARGS +=	-DCOPY_EXE=Off

# # for $REASON almost all needed -lflags are missing.  go figure.
# LDFLAGS +=		-L${LOCALBASE}/lib -L${X11BASE}/lib \
# 			-ljpeg \
# 			-lavcodec -lavdevice -lavfilter -lavformat -lavresample -lavutil \
# 			-lvorbisfile \
# 			-lswresample \
# 			-lswscale \
# 			-lquazip5 \
# 			-lopenal \
# 			-lpugixml \
# 			-lcurl \
# 			-lz \
# 			-lexecinfo

# "CC=c++" works around some sloppiness from upstream,
# but is deprecated for C code
# MODSCONS_FLAGS +=	BUILD="release" \
# 			BASECPPFLAGS="${CXXFLAGS}" \
# 			CC="c++" \
# 			CPPPATH="-I${LOCALBASE}/include -I${X11BASE}/include" \
# 			CXX="c++" \
# 			LINKFLAGS="${LDFLAGS}" \
# 			NOCURL=1 \
# 			LIBC_MALLOC=1

# .if ${MACHINE_ARCH} == "amd64"
# MODSCONS_FLAGS +=	TARGET_ARCH="x64"
# .elif ${MACHINE_ARCH} == "i386"
# MODSCONS_FLAGS +=	TARGET_ARCH="i386"
# .endif

# remove bundled dependencies to make sure they are not picked up
post-extract:
#	rm -rf ${WRKSRC}/ThirdParty/artefacts/zlib
#	rm -rf ${WRKSRC}/framework/minizip
	rm -rf ${WRKSRC}/include/boost

# do-gen:
# 	${SUBST_CMD} ${WRKSRC}/SConstruct
# 	${SUBST_CMD} ${WRKSRC}/tdm_update/SConscript.libtdm_update
# 	${SUBST_CMD} ${WRKSRC}/tdm_update/SConstruct

# do-build:
# 	${SETENV} ${MAKE_ENV} ${MODSCONS_BIN} -C ${WRKSRC}/tdm_update \
# 		${MODSCONS_ENV} ${MODSCONS_FLAGS} ${ALL_TARGET}
# 	${SETENV} ${MAKE_ENV} ${MODSCONS_BIN} -C ${WRKSRC} \
# 		${MODSCONS_ENV} ${MODSCONS_FLAGS} ${ALL_TARGET}

# ALL_TARGET =		TheDarkMod

do-install:
	${INSTALL_DATA_DIR} ${PREFIX}/share/darkmod
	${INSTALL_PROGRAM} ${WRKBUILD}/thedarkmod.x64 \
		${PREFIX}/share/darkmod/thedarkmod
	${INSTALL_DATA} ${FILESDIR}/Darkmod.cfg ${PREFIX}/share/darkmod/

#	${INSTALL_PROGRAM} ${WRKBUILD}/thedarkmod ${PREFIX}/share/thedarkmod/
#	${INSTALL_PROGRAM} ${WRKBUILD}/tdm_update/bin/tdm_update.linux64 \
#		${PREFIX}/share/thedarkmod/tdm_update

post-install:
	${SUBST_CMD} -c -m 755 ${FILESDIR}/thedarkmod.sh ${PREFIX}/bin/thedarkmod
	${SUBST_CMD} -c -m 755 ${FILESDIR}/tdm_update.sh ${PREFIX}/bin/tdm_update

.include <bsd.port.mk>
