# $OpenBSD$

COMMENT =	lightweight audio filtering library made from webrtc code

V =		0.0.1
DISTNAME =	filter_audio-${V}
PKGNAME =	libfilteraudio-${V}

GH_ACCOUNT =	irungentoo
GH_PROJECT =	filter_audio
GH_TAGNAME =	v${V}

SHARED_LIBS =	filteraudio	0.0

CATEGORIES =	audio

# BSD3
PERMIT_PACKAGE =	Yes

WANTLIB =		m pthread

# VERSION is used to generate the so version.
MAKE_FLAGS +=		VERSION=0.0

FAKE_FLAGS =		PREFIX=${PREFIX}

USE_GMAKE =		Yes

.include <bsd.port.mk>
