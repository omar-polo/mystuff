# $OpenBSD$

COMMENT =	cross-platform library for building Telegram clients

V =		83c9f5180bf9738df2667841ed0c4df9ebeaec40
DISTNAME =	td-${V}
PKGNAME =	td-1.6beta6

GH_ACCOUNT =	tdlib
GH_PROJECT =	td
# GH_TAGNAME =	v1.6.0
GH_COMMIT =	${V}

SHARED_LIBS +=	tdjson 0.0	# 1.6.0
CATEGORIES =	net

# GPLv3
PERMIT_PACKAGE = Yes

MODULES =	devel/cmake

COMPILER=base-clang ports-gcc

WANTLIB += ${COMPILER_LIBCXX} crypto m ssl z

.include <bsd.port.mk>
