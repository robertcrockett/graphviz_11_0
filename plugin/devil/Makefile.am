## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt \
	$(PANGOCAIRO_CFLAGS) $(DEVIL_CFLAGS)

if WITH_DEVIL
noinst_LTLIBRARIES = libgvplugin_devil_C.la
if WITH_WIN32
lib_LTLIBRARIES = libgvplugin_devil.la
else
pkglib_LTLIBRARIES = libgvplugin_devil.la
endif
endif

libgvplugin_devil_C_la_SOURCES = \
	gvplugin_devil.c \
	gvdevice_devil.c

libgvplugin_devil_la_LDFLAGS = -version-info $(GVPLUGIN_VERSION_INFO)
libgvplugin_devil_la_SOURCES = $(libgvplugin_devil_C_la_SOURCES)
libgvplugin_devil_la_LIBADD = $(DEVIL_LIBS)

if WITH_WIN32
libgvplugin_devil_la_LDFLAGS += -no-undefined
endif
