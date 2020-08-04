# $OpenBSD$

COMMENT =	StarDict console version

GH_ACCOUNT =	Dushistov
GH_PROJECT =	sdcv
GH_TAGNAME =	v0.5.2

CATEGORIES =	textproc

HOMEPAGE =	https://dushistov.github.io/sdcv/

# GPLv2
PERMIT_PACKAGE =	Yes

WANTLIB += ${COMPILER_LIBCXX} c glib-2.0 intl m readline z

# for C++11
COMPILER =		base-clang ports-gcc

MODULES =		devel/cmake

LIB_DEPENDS =		devel/glib2

TEST_DEPENDS =		textproc/jq

CONFIGURE_ARGS +=	-DENABLE_NLS=False \
			-DBUILD_TESTS=True

.include <bsd.port.mk>
