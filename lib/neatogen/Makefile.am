## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/pack \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt $(IPSEPCOLA_INCLUDES) $(GTS_CFLAGS)

if WITH_WIN32
AM_CFLAGS = -DNEATOGEN_EXPORTS=1
endif

noinst_LTLIBRARIES = libneatogen_C.la

noinst_HEADERS = adjust.h edges.h geometry.h heap.h hedges.h info.h mem.h \
	neato.h poly.h neatoprocs.h site.h voronoi.h \
	bfs.h closest.h conjgrad.h defs.h dijkstra.h embed_graph.h kkutils.h \
	matrix_ops.h pca.h stress.h quad_prog_solver.h digcola.h \
	overlap.h call_tri.h \
	quad_prog_vpsc.h delaunay.h sparsegraph.h multispline.h fPQ.h \
	sgd.h randomkit.h

IPSEPCOLA_SOURCES = constrained_majorization_ipsep.c quad_prog_vpsc.c

if WITH_IPSEPCOLA
WITH_IPSEPCOLA_SOURCES = $(IPSEPCOLA_SOURCES)
endif

libneatogen_C_la_SOURCES = adjust.c circuit.c edges.c geometry.c \
	heap.c hedges.c info.c neatoinit.c legal.c lu.c matinv.c \
	memory.c poly.c site.c solve.c neatosplines.c stuff.c \
	voronoi.c stress.c kkutils.c matrix_ops.c embed_graph.c dijkstra.c \
	conjgrad.c pca.c closest.c bfs.c constraint.c quad_prog_solve.c \
	smart_ini_x.c constrained_majorization.c opt_arrangement.c \
	overlap.c call_tri.c \
	compute_hierarchy.c delaunay.c multispline.c $(WITH_IPSEPCOLA_SOURCES) \
	sgd.c randomkit.c

EXTRA_DIST = $(IPSEPCOLA_SOURCES) gvneatogen.vcxproj*
