## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt \
	$(POPPLER_CFLAGS) $(PANGOCAIRO_CFLAGS)

if WITH_POPPLER
if WITH_PANGOCAIRO
noinst_LTLIBRARIES = libgvplugin_poppler_C.la
if WITH_WIN32
lib_LTLIBRARIES = libgvplugin_poppler.la
else
pkglib_LTLIBRARIES = libgvplugin_poppler.la
endif
endif
endif

libgvplugin_poppler_C_la_SOURCES = \
	gvplugin_poppler.c \
	gvloadimage_poppler.c

libgvplugin_poppler_la_LDFLAGS = -version-info $(GVPLUGIN_VERSION_INFO)
libgvplugin_poppler_la_SOURCES = $(libgvplugin_poppler_C_la_SOURCES)
libgvplugin_poppler_la_LIBADD = \
	$(top_builddir)/lib/gvc/libgvc.la $(PANGOCAIRO_LIBS) $(POPPLER_LIBS)

if WITH_WIN32
libgvplugin_poppler_la_LDFLAGS += -no-undefined
endif

EXTRA_DIST =
