## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt \
	$(PANGOCAIRO_CFLAGS)

if WITH_PANGOCAIRO
if WITH_X
noinst_LTLIBRARIES = libgvplugin_xlib_C.la
if WITH_WIN32
lib_LTLIBRARIES = libgvplugin_xlib.la
else
pkglib_LTLIBRARIES = libgvplugin_xlib.la
endif
bin_SCRIPTS = vimdot
man_MANS = vimdot.1
if ENABLE_MAN_PDFS
pdf_DATA = vimdot.1.pdf
endif
endif
endif

.1.1.pdf:
	rm -f $@; pdffile=$@; psfile=$${pdffile%pdf}ps; \
	$(GROFF) -Tps -man $< > $$psfile || { rm -f $$psfile; exit 1; }; \
	$(PS2PDF) $$psfile && rm -f $$psfile || { rm -f $$psfile; exit 1; }
SUFFIXES = .1 .1.pdf

libgvplugin_xlib_C_la_SOURCES = \
	gvplugin_xlib.c \
	gvdevice_xlib.c

libgvplugin_xlib_la_LDFLAGS = -version-info $(GVPLUGIN_VERSION_INFO)
libgvplugin_xlib_la_SOURCES = $(libgvplugin_xlib_C_la_SOURCES)
libgvplugin_xlib_la_LIBADD = $(PANGOCAIRO_LIBS) $(X11_LIBS) $(XRENDER_LIBS) $(MATH_LIBS)

if WITH_WIN32
libgvplugin_xlib_la_LDFLAGS += -no-undefined
endif

vimdot: $(top_srcdir)/plugin/xlib/vimdot.sh
	cp $(top_srcdir)/plugin/xlib/vimdot.sh vimdot
	chmod +x vimdot

EXTRA_DIST = vimdot.sh vimdot.1

DISTCLEANFILES = vimdot $(pdf_DATA)
