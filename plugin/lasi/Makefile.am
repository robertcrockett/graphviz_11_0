## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt \
	$(LASI_CFLAGS) $(PANGOCAIRO_CFLAGS) $(FREETYPE2_CFLAGS) $(LIBGVC_CFLAGS)

if WITH_LASI
if WITH_PANGOCAIRO
noinst_LTLIBRARIES = libgvplugin_lasi_C.la
if WITH_WIN32
lib_LTLIBRARIES = libgvplugin_lasi.la
else
pkglib_LTLIBRARIES = libgvplugin_lasi.la
endif
endif
endif

libgvplugin_lasi_C_la_SOURCES = \
	gvplugin_lasi.c \
	gvrender_lasi.cpp \
	gvloadimage_lasi.cpp

libgvplugin_lasi_la_LDFLAGS = -version-info $(GVPLUGIN_VERSION_INFO)
libgvplugin_lasi_la_SOURCES = $(libgvplugin_lasi_C_la_SOURCES)
libgvplugin_lasi_la_LIBADD = \
	$(top_builddir)/lib/gvc/libgvc.la $(LASI_LIBS) $(PANGOCAIRO_LIBS) $(MATH_LIBS)

if WITH_WIN32
libgvplugin_lasi_la_LDFLAGS += -no-undefined
endif
