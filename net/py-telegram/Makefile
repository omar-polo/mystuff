# $OpenBSD$

COMMENT =	python wrapper for Telegram tdlib

MODPY_EGG_VERSION =	0.13.0

DISTNAME =	python-telegram-${MODPY_EGG_VERSION}
PKGNAME =	${DISTNAME:S/^python-/py-/}

CATEGORIES =	net

HOMEPAGE =	https://github.com/alexander-akhmetov/python-telegram

# MIT
PERMIT_PACKAGE =	Yes

MODULES =		lang/python

MODPY_PI =		Yes
MODPY_VERSION =		${MODPY_DEFAULT_VERSION_3}

RUN_DEPENDS =		net/td

post-install:
	cd ${WRKINST} && rm -rf usr/local/lib/python${MODPY_VERSION}/site-packages/telegram/lib

.include <bsd.port.mk>
