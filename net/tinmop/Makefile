# $OpenBSD$

#		|----------------------------------------------------------|
COMMENT =	opinionated client for Pleroma and the Gemini protocol

V =		0.5.9
#DISTNAME =	v${V}
DISTNAME =	6228858e79
PKGNAME =	tinmop-${V}

# XXX: tmp
WRKSRC =	${WRKDIR}/tinmop

#SHARED_LIBS =	???	0.0

CATEGORIES =	net

HOMEPAGE =	https://notabug.org/cage/tinmop.git

MAINTAINER =	Omar Polo <op@omarpolo.com>

# GPLv3
PERMIT_PACKAGE =	Yes

# "make port-lib-depends-check" can help
#WANTLIB =		???

MASTER_SITES =		https://notabug.org/cage/tinmop/archive/
MASTER_SITES0 =		http://beta.quicklisp.org/archive/

DISTFILES =		${DISTNAME}${EXTRACT_SUFX} \
			{alexandria/2020-09-25/}alexandria-20200925-git.tgz:0 \
			{cl-ppcre/2019-05-21/}cl-ppcre-20190521-git.tgz:0 \
			{tooter/2020-12-20/}tooter-20201220-git.tgz:0 \
			{croatoan/2021-01-24/}croatoan-20210124-git.tgz:0 \
			{osicat/2020-09-25/}osicat-20200925-git.tgz:0 \
			{cl-spark/2015-07-09/}cl-spark-20150709-git.tgz:0 \
			{access/2021-01-24/}access-20210124-git.tgz:0 \
			{cl-sqlite/2019-08-13/}cl-sqlite-20190813-git.tgz:0 \
			{sxql/2020-12-20/}sxql-20201220-git.tgz:0 \
			{sxql-composer/2020-03-25/}sxql-composer-20200325-git.tgz:0 \
			{cl-marshal/2020-10-16/}cl-marshal-20201016-git.tgz:0 \
			{bordeaux-threads/2020-06-10/}bordeaux-threads-v0.8.8.tgz:0 \
			{cl-base64/2020-10-16/}cl-base64-20201016-git.tgz:0 \
			{cl+ssl/2021-01-24/}cl+ssl-20210124-git.tgz:0 \
			{log4cl/2020-09-25/}log4cl-20200925-git.tgz:0 \
			{local-time/2021-01-24/}local-time-20210124-git.tgz:0 \
			{cl-colors2/2020-02-18/}cl-colors2-20200218-git.tgz:0 \
			{cl-i18n/2020-07-15/}cl-i18n-20200715-git.tgz:0 \
			{clunit2/2020-10-16/}clunit2-20201016-git.tgz:0 \
			{esrap/2020-12-20/}esrap-20201220-git.tgz:0 \
			{ieee-floats/2017-08-30/}ieee-floats-20170830-git.tgz:0 \
			{parse-number/2018-02-28/}parse-number-v1.7.tgz:0 \
			{cl-html5-parser/2019-05-21/}cl-html5-parser-20190521-git.tgz:0 \
			{unix-opts/2021-01-24/}unix-opts-20210124-git.tgz:0 \
			{crypto-shortcuts/2020-10-16/}crypto-shortcuts-20201016-git.tgz:0 \
			{drakma/2019-11-30/}drakma-v2.0.7.tgz:0 \
			{usocket/2019-12-27/}usocket-0.8.3.tgz:0 \
			{babel/2020-09-25/}babel-20200925-git.tgz:0 \
			{percent-encoding/2012-10-13/}percent-encoding-20121013-git.tgz:0 \
			{uiop/2020-02-18/}uiop-3.3.4.tgz:0 \

BUILD_DEPENDS =		devel/gettext,-runtime \
			lang/sbcl

DESTDIRNAME =		destdir
WRKDIST =		${WRKSRC}

USE_GMAKE =		Yes

CONFIGURE_STYLE =	gnu

AUTOCONF_VERSION =	2.69
AUTOMAKE_VERSION =	1.16

CONFIGURE_ENV +=	LANG=en_US.UTF-8
MAKE_ENV +=		LANG=en_US.UTF-8

pre-configure:
	find ${WRKDIR} -type f -name \*.asd -a \! -name tinmop.asd \
		-exec ln -s {} ${WRKSRC} \;

# post-configure:
#	cd ${WRKDIR} && sed '/^LISP_COMPILER =/ s/$/ --eval '

.include <bsd.port.mk>
