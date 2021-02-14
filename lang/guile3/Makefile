# $OpenBSD$

COMMENT=		GNU's Ubiquitous Intelligent Language for Extension
# '

VERSION=		3.0.5
DISTNAME=		guile-${VERSION}
PKGNAME=		guile3-${VERSION}
V=			${VERSION:R}
SUBST_VARS=		V

CATEGORIES=		lang

HOMEPAGE=		https://www.gnu.org/software/guile/

SHARED_LIBS +=  guile-3.0            0.0      # 3.0.5

# GPLv3, LGPLv3
PERMIT_PACKAGE=		Yes

WANTLIB += c curses ffi gc gmp iconv intl ltdl m pthread readline
WANTLIB += unistring

MASTER_SITES=		${MASTER_SITE_GNU:=guile/}

# internal compiler error: in unify_nodes, at tree-ssa-structalias.c:1231
COMPILER =		base-clang ports-gcc
COMPILER_LANGS =	c

USE_GMAKE=		Yes

RUN_DEPENDS=		devel/slib
LIB_DEPENDS=		converters/libunistring \
			devel/boehm-gc \
			devel/gettext,-runtime \
			devel/gmp \
			devel/libffi \
			devel/libtool,-ltdl

CONFIGURE_STYLE=	gnu
CONFIGURE_ENV=		CPPFLAGS="-I${LOCALBASE}/include" \
			LDFLAGS="-L${LOCALBASE}/lib"
CONFIGURE_ARGS=		--enable-jit=no \
			--program-suffix=${V}

# powerpc: Prevent "Segmentation fault (core dumped)" during build.
post-patch:
	mv ${WRKSRC}/prebuilt/32-bit-big-endian{,-broken}

post-install:
	install -d ${PREFIX}/share/guile/site/${V}/
	# fix conflict with devel/guile
	mv ${PREFIX}/share/aclocal/guile{,${V}}.m4

.include <bsd.port.mk>
