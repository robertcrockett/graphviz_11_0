## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt \
	$(PANGOCAIRO_CFLAGS) $(WEBP_CFLAGS)

if WITH_WEBP
noinst_LTLIBRARIES = libgvplugin_webp_C.la
if WITH_WIN32
lib_LTLIBRARIES = libgvplugin_webp.la
else
pkglib_LTLIBRARIES = libgvplugin_webp.la
endif
endif

libgvplugin_webp_C_la_SOURCES = \
	gvplugin_webp.c \
	gvloadimage_webp.c \
	gvdevice_webp.c

libgvplugin_webp_la_LDFLAGS = -version-info $(GVPLUGIN_VERSION_INFO)
libgvplugin_webp_la_SOURCES = $(libgvplugin_webp_C_la_SOURCES)
libgvplugin_webp_la_LIBADD = $(WEBP_LIBS) $(PANGOCAIRO_LIBS) \
	$(top_builddir)/lib/gvc/libgvc.la

if WITH_WIN32
libgvplugin_webp_la_LDFLAGS += -no-undefined
endif
