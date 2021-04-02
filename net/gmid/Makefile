# $OpenBSD$

COMMENT =	simple and secure Gemini server

GH_ACCOUNT =	omar-polo
GH_PROJECT =	gmid
GH_TAGNAME =	v1.6

CATEGORIES =	net

HOMEPAGE =	https://www.omarpolo.com/pages/gmid.html

MAINTAINER =		Omar Polo <op@omarpolo.com>

# ISC
PERMIT_PACKAGE =	Yes

# uses pledge()
# uses unveil()
WANTLIB += c crypto event ssl tls util

CONFIGURE_STYLE =	simple

TEST_TARGET =		regress

.include <bsd.port.mk>
