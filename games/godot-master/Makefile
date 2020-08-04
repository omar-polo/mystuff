# $OpenBSD: Makefile,v 1.10 2020/04/12 13:30:55 cwen Exp $

COMMENT =	2D and 3D game engine

V =		4.0pre
DISTNAME =	godot-${V}-stable
PKGNAME =	godot-${V}
CATEGORIES =	games
HOMEPAGE =	https://godotengine.org/
MAINTAINER =	Thomas Frohwein <thfr@openbsd.org>

DEBUG_PACKAGES=${BUILD_PACKAGES}

# MIT
PERMIT_PACKAGE =	Yes

WANTLIB += ${COMPILER_LIBCXX}
WANTLIB += GL X11 Xau Xcursor Xdmcp Xext Xfixes Xi Xinerama Xrandr
WANTLIB += Xrender c crypto enet execinfo freetype intl m mbedtls
WANTLIB += mbedcrypto mbedx509 mpcdec ogg opus opusfile png theora
WANTLIB += theoradec vorbis vorbisfile webp xcb z pcre2-32 vpx zstd

COMPILER =		base-clang ports-gcc base-gcc

# https://github.com/godotengine/godot
GH_ACCOUNT =	godotengine
GH_PROJECT =	godot
GH_COMMIT =	0d4838950dfcc0d462e8571e54f9cb8221fc6d67

DEBUG_PACKAGES=${BUILD_PACKAGES}

MODULES =		devel/scons
# Can't disable builtin_bullet until devel/bullet has been updated to 2.88
# Building with module_mono_enabled requires msbuild (which we don't have
# yet) and to fix the sharedlib_ext in modules/mono/config.py to '.so.1.0'
# Disable pulseaudio which is currently not working
MODSCONS_FLAGS =	CC="${CC}" \
			CXX="${CXX}" \
			CFLAGS="${CFLAGS} -Wno-register" \
			CXXFLAGS="${CXXFLAGS}" \
			LINKFLAGS="${LDFLAGS} -lintl -lmpcdec" \
			builtin_enet=no \
			builtin_freetype=no \
			builtin_gglslang=no \
			builtin_libogg=no \
			builtin_libpng=no \
			builtin_libtheora=no \
			builtin_libvorbis=no \
			builtin_libvpx=no \
			builtin_libwebp=no \
			builtin_wslay=no \
			builtin_mbedtls=no \
			builtin_miniupnpc=no \
			builtin_opus=no \
			builtin_pcre2=no \
			builtin_pcre2_with_jit=no \
			builtin_vulkan=no \
			builtin_zlib=no \
			builtin_zstd=no \
			dev=yes \
			platform=linuxbsd \
			progress=no \
			pulseaudio=no \
			target=release_debug \
			warnings=no \
			werror=no
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

CONFIGURE_ENV +=	CXXFLAGS="${CXXFLAGS} -Wno-register" \
			warnings="no"

post-extract:
	cp ${FILESDIR}/vk_mem_alloc.h ${WRKSRC}/platform/linuxbsd
	cp ${FILESDIR}/vk_enum_string_helper.h ${WRKSRC}/platform/linuxbsd

pre-configure:
	${SUBST_CMD} ${WRKSRC}/drivers/unix/os_unix.cpp

do-install:
	${INSTALL_PROGRAM} ${WRKBUILD}/bin/godot* \
		${PREFIX}/bin/godot

# for maintainer
godot-fetch-headers:
	cd files && ftp https://raw.githubusercontent.com/KhronosGroup/Vulkan-ValidationLayers/master/layers/generated/vk_enum_string_helper.h
	cd files && ftp https://raw.githubusercontent.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/master/src/vk_mem_alloc.h

.include <bsd.port.mk>
