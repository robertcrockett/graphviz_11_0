## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt \
	-I$(top_srcdir)/lib/gvpr \
	-I$(top_srcdir)/lib/xdot \
	$(LIBGVC_CFLAGS)

noinst_LTLIBRARIES = libgvplugin_core_C.la
if WITH_WIN32
lib_LTLIBRARIES = libgvplugin_core.la
else
pkglib_LTLIBRARIES = libgvplugin_core.la
endif

libgvplugin_core_C_la_SOURCES = \
	gvplugin_core.c \
	gvrender_core_dot.c \
	gvrender_core_json.c \
	gvrender_core_fig.c \
	gvrender_core_map.c \
	gvrender_core_mp.c \
	gvrender_core_ps.c \
	gvrender_core_svg.c \
	gvrender_core_tk.c \
	gvrender_core_pov.c \
	gvrender_core_pic.c \
	gvloadimage_core.c

libgvplugin_core_la_LDFLAGS = -version-info $(GVPLUGIN_VERSION_INFO)
libgvplugin_core_la_SOURCES = $(libgvplugin_core_C_la_SOURCES)
libgvplugin_core_la_LIBADD = $(top_builddir)/lib/gvc/libgvc.la \
	$(top_builddir)/lib/cgraph/libcgraph.la \
	$(top_builddir)/lib/cdt/libcdt.la \
	$(top_builddir)/lib/xdot/libxdot.la

if WITH_WIN32
libgvplugin_core_la_LDFLAGS += -no-undefined
libgvplugin_core_la_LIBADD += $(top_builddir)/lib/cgraph/libcgraph.la
endif

EXTRA_DIST = ps.h gvplugin_core.vcxproj*
