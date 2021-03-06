# $OpenBSD: Makefile,v 1.11 2020/07/19 13:02:38 thfr Exp $

COMMENT =	2D and 3D game engine

V =		3.2.2
DISTNAME =	godot-${V}-stable
PKGNAME =	godot-${V}
CATEGORIES =	games
HOMEPAGE =	https://godotengine.org/
MAINTAINER =	Thomas Frohwein <thfr@openbsd.org>
REVISION =	1

# MIT
PERMIT_PACKAGE =	Yes

WANTLIB += ${COMPILER_LIBCXX}
WANTLIB += GL X11 Xau Xcursor Xdmcp Xext Xfixes Xi Xinerama Xrandr
WANTLIB += Xrender c enet execinfo freetype intl m mbedtls mbedcrypto
WANTLIB += mbedx509 mpcdec ogg opus opusfile png sndio theora theoradec
WANTLIB += vorbis vorbisfile webp xcb z pcre2-32 vpx zstd

COMPILER =		base-clang ports-gcc base-gcc

MASTER_SITES =	https://downloads.tuxfamily.org/godotengine/${V}/
EXTRACT_SUFX =	.tar.xz

MODULES =		devel/scons
# Can't disable builtin_bullet until devel/bullet has been updated to 2.88
# Building with module_mono_enabled requires msbuild (which we don't have
# yet) and to fix the sharedlib_ext in modules/mono/config.py to '.so.1.0'
# Disable pulseaudio which is currently not working
MODSCONS_FLAGS =	CC="${CC}" \
			CXX="${CXX}" \
			CFLAGS="${CFLAGS}" \
			CXXFLAGS="${CXXFLAGS} -Wno-deprecated-register" \
			LINKFLAGS="${LDFLAGS} -lintl -lmpcdec" \
			builtin_enet=no \
			builtin_freetype=no \
			builtin_glew=no \
			builtin_libmpcdec=no \
			builtin_libogg=no \
			builtin_libpng=no \
			builtin_libtheora=no \
			builtin_libvorbis=no \
			builtin_libvpx=no \
			builtin_libwebp=no \
			builtin_mbedtls=no \
			builtin_opus=no \
			builtin_pcre2=no \
			builtin_zlib=no \
			builtin_zstd=no \
			dev=yes \
			platform=x11 \
			progress=no \
			pulseaudio=no \
			target=release_debug
LIB_DEPENDS =		archivers/zstd \
			audio/libvorbis \
			audio/musepack \
			audio/opusfile \
			devel/gettext,-runtime \
			devel/libexecinfo \
			devel/pcre2 \
			graphics/libwebp \
			graphics/png \
			multimedia/libtheora \
			multimedia/libvpx \
			net/enet \
			security/polarssl

NO_TEST =		Yes

LDFLAGS +=	-lsndio

.if ${MACHINE_ARCH:Mhppa}
LDFLAGS +=     -latomic
WANTLIB +=     atomic
.endif

# Fix relocation overflows
.if ${MACHINE_ARCH:Mpowerpc}
CFLAGS +=      -mlongcall
CXXFLAGS +=    -mlongcall
LDFLAGS +=     -Wl,--relax
.endif

post-extract:
	cp -R ${FILESDIR}/sndio ${WRKDIST}/drivers

pre-configure:
	${SUBST_CMD} ${WRKSRC}/drivers/unix/os_unix.cpp

do-install:
	${INSTALL_PROGRAM} ${WRKBUILD}/bin/godot* \
		${PREFIX}/bin/godot

.include <bsd.port.mk>
