# $OpenBSD$

COMMENT =	cross-platform library for building Telegram clients

V =		7a59b3d2b36869b11fb1f4f43e678856a26f8deb
DISTNAME =	td-${V}
PKGNAME =	td-1.6.9

GH_ACCOUNT =	tdlib
GH_PROJECT =	td
GH_COMMIT =	${V}

SHARED_LIBS +=	tdjson 0.0	# 1.6.9
CATEGORIES =	net

# Boost SL 1.0
PERMIT_PACKAGE = Yes

MODULES =	devel/cmake

BUILD_DEPENDS =	devel/gperf

COMPILER =	base-clang ports-gcc

WANTLIB +=	${COMPILER_LIBCXX} crypto m ssl z

SUBST_VARS +=	GH_COMMIT

post-patch:
	${SUBST_CMD} ${WRKSRC}/gen_git_commit_h.sh

.include <bsd.port.mk>
