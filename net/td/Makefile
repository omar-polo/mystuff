# $OpenBSD$

# XXX: if the build fails due to OOM errors, see the tdlib README in
# the "Building" section on how to use SplitSource.php to reduce the
# memory usage.

COMMENT =	cross-platform library for building Telegram clients

# XXX: because of GH_COMMIT
PKGNAME =	tdlib-1.7.1
DISTNAME =	tdlib-${GH_COMMIT}

GH_ACCOUNT =	tdlib
GH_PROJECT =	td
#GH_TAGNAME =	v1.7.0		# XXX: keep in sync with COMMIT below
GH_COMMIT =	3a657d9072b324aad1143c6abe20a802f3b92cd5

SHARED_LIBS +=	tdjson	0.0	# 1.7.1
CATEGORIES =	net

MAINTAINER =	Omar Polo <op@omarpolo.com>

# Boost SL 1.0
PERMIT_PACKAGE = Yes

MODULES =	devel/cmake

BUILD_DEPENDS =	devel/gperf

# C++14
COMPILER =	base-clang ports-gcc

WANTLIB +=	${COMPILER_LIBCXX} crypto m ssl z

COMMIT =	3a657d9072b324aad1143c6abe20a802f3b92cd5
SUBST_VARS +=	COMMIT

post-patch:
	${SUBST_CMD} ${WRKSRC}/gen_git_commit_h.sh

.include <bsd.port.mk>
