# $OpenBSD$

COMMENT =	cross-platform library for building Telegram clients

GH_ACCOUNT =	tdlib
GH_PROJECT =	td
GH_TAGNAME =	v1.6.0

CATEGORIES =	net

# GPLv3
PERMIT_PACKAGE = Yes

MODULES =	devel/cmake

COMPILER=base-clang ports-gcc

WANTLIB += ${COMPILER_LIBCXX} crypto m ssl z

.include <bsd.port.mk>
