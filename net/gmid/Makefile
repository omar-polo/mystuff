# $OpenBSD$

COMMENT =	simple and secure Gemini server

# only because of GH_COMMIT
PKGNAME =	gmid-1.6beta
DISTNAME =	gmid

GH_ACCOUNT =	omar-polo
GH_PROJECT =	gmid
#GH_TAGNAME =	v1.6
GH_COMMIT =	fda7b99fc7f19b04eced114983268cfe3eb46c99

CATEGORIES =	net

HOMEPAGE =	https://www.omarpolo.com/pages/gmid.html

MAINTAINER =		Omar Polo <op@omarpolo.com>

# ISC
PERMIT_PACKAGE =	Yes

# uses pledge()

WANTLIB += c crypto event ssl tls util

CONFIGURE_STYLE =	simple

TEST_TARGET =		regress

.include <bsd.port.mk>
