/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <neatogen/voronoi.h>
#include <neatogen/poly.h>

    typedef struct ptitem {	/* Point list */
	struct ptitem *next;
	Point p;
    } PtItem;

    typedef struct {		/* Info concerning site */
	Agnode_t *node;		/* libgraph node */
	Site site;		/* site used by voronoi code */
	int overlaps;		/* true if node overlaps other nodes */
	Poly poly;		/* polygon at node */
	PtItem *verts;		/* sorted list of vertices of */
	/* voronoi polygon */
    } Info_t;

    extern Info_t *nodeInfo;	/* Array of node info */

    extern void infoinit(void);
    /* Insert vertex into sorted list */
    extern void addVertex(Site *, double, double);

#ifdef __cplusplus
}
#endif
