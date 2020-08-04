# $OpenBSD$

#
# Very short description of the package (<=60 chars, usually starts lower-case)
#		|----------------------------------------------------------|
COMMENT =	mesh parametrization and UV unwrapping library

DISTNAME =	???
# PKGNAME defaults to DISTNAME unless changed
#PKGNAME =	???
#PKGNAME-foo =	???	for multi packages

GH_ACCOUNT =	jpcy
GH_PROJECT =	xatlas
GH_COMMIT =	470576d3516f7e6d8b4554e7c941194a935969fd

# for any port that creates shared libraries.
# both libtool and cmake automatically set filenames based on this variable.
# for libtool- or cmake-based ports, have a look at WRKBUILD/shared_libs.log
SHARED_LIBS =	???	0.0

# the category used in the directory name, /usr/ports/<category>/portname,
# must be included and listed first.
CATEGORIES =	dev

# https preferred for HOMEPAGE
HOMEPAGE =	https://github.com/jpcy/xatlas

MAINTAINER =		Omar Polo <op@omarpolo.com>

# MIT
PERMIT_PACKAGE =	Yes

# "make port-lib-depends-check" can help
#WANTLIB =		???

# where the source files and patches can be fetched
#
#MASTER_SITES =		???
#MASTER_SITES =		${MASTER_SITE_SOURCEFORGE:=subdir/}
#MASTER_SITES =		${MASTER_SITE_foo:=subdir/}

# Needs to be specified if tarball does not end with .tar.gz
#EXTRACT_SUFX =		.tar.bz2

# Optional subdirectory of DISTDIR where distfiles and patches will be placed
#DIST_SUBDIR =		???

COMPILER =		base-clang ports-gcc

# Any modules we may be using
#MODULES =		???

# Dependencies
#BUILD_DEPENDS =	???
#RUN_DEPENDS =		???
#LIB_DEPENDS =		???
#TEST_DEPENDS =		???

#MAKE_FLAGS =		???
#MAKE_ENV =		???
#FAKE_FLAGS =		???
#TEST_FLAGS =		???

# build/configuration variables
#
#SEPARATE_BUILD =	Yes (build in a directory other than WRKSRC)
#SEPARATE_BUILD =	flavored (distinct flavors may share a common WRKSRC)
#USE_GMAKE =		Yes
#USE_GROFF =		Yes
# Programs that require GNU libtool to build instead of the OpenBSD one
# should use this option. Add a comment explaining why. Don't use this if
# a port requires libtool's .m4 files but otherwise can use OpenBSD libtool,
# in that case use "BUILD_DEPENDS=devel/libtool" instead.
#USE_LIBTOOL=		gnu
# Set CONFIGURE_STYLE to value corresponding to some standard configuration
#	  perl [modbuild]: perl's MakeMaker Makefile.PL (modbuild: perl's
#	  Module::Build Build.PL)
#	  gnu [autoconf] [old] [dest]: gnu style configure (old: no
#	  sysconfdir), (dest: add DESTDIR, does not handle it),
#	  autoconf: run autoconf to regenerate configure script. implies gnu.
#	            (see also "do-gen" target below).
#	XXX: cygnus products do NOT use autoconf for making the main
#		configure from configure.in
#	  imake [noman]: port uses imake for configuration.
#	  (noman: no man page installation)
#	  simple: port has its own configure script
#	  none: override default CONFIGURE_STYLE coming from a module
#	        (needed for some ports using lang/python, etc.)
#CONFIGURE_STYLE =	
#CONFIGURE_SCRIPT =	??? (if other than configure)
#CONFIGURE_ARGS =	???
#CONFIGURE_ENV =	???

# if debug packages are a good idea, extra configure args may be necessary
# and DEBUG_PACKAGES will commonly be set to ${BUILD_PACKAGES}
# DEBUG_CONFIGURE_ARGS =	???
# DEBUG_PACKAGES = ${BUILD_PACKAGES}

# Is the build automagic or is it interactive
#
#IS_INTERACTIVE =		Yes
#TEST_IS_INTERACTIVE =		Yes

NO_TEST =		Yes

# Overrides for default values
#
#CFLAGS =		???
#LDFLAGS =		???
#MAKE_FILE =		???
#PKG_ARCH =		??? (* for arch-independent packages)
#WRKDIST =		??? if other than ${WRKDIR}/${DISTNAME}
#WRKSRC =		??? if other than ${WRKDIST}
#WRKBUILD =		??? if other than ${WRKSRC}
#WRKCONF =		??? if other than ${WRKBUILD}

#ALL_TARGET =		???
#INSTALL_TARGET =	???
#TEST_TARGET =		???

# For ports that use a script or autoreconf to generate autoconf/automake
# files (where "CONFIGURE_STYLE=autoconf" isn't enough), use some/all of these
# dependencies, and add a do-gen target:
# 
#BUILD_DEPENDS =	${MODGNU_AUTOCONF_DEPENDS} \
#			${MODGNU_AUTOMAKE_DEPENDS} \
#			devel/libtool
#
#do-gen:
#	cd ${WRKSRC}; ${AUTOCONF_ENV} ./autogen.sh

# Fix dos line-endings in files needing patches
#pre-patch:
#	@cd ${WRKSRC} && perl -i -pe 's/\r$$//' [...]

.include <bsd.port.mk>
