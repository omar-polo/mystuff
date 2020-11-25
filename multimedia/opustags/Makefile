# $OpenBSD: Makefile $

COMMENT =		tags editor for ogg opus files

GH_ACCOUNT =		fmang
GH_PROJECT =		opustags
GH_TAGNAME =		1.5.0

CATEGORIES =		audio multimedia

# BSD3
PERMIT_PACKAGE =	Yes

WANTLIB =		${COMPILER_LIBCXX} c m iconv iconv ogg

# C++17
COMPILER =		base-clang ports-gcc

MODULES =		devel/cmake

LIB_DEPENDS =		audio/libogg \
			converters/libiconv

TEST_DEPENDS =		devel/p5-List-MoreUtils
TEST_TARGET =		check
TEST_ENV =		LC_CTYPE=en_US.UTF-8

.include <bsd.port.mk>
