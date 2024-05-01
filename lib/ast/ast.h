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

#include <cgraph/agxbuf.h>
#include <sfio/sfio.h>
#include <stdlib.h>
#include <string.h>

/*
 * strgrpmatch() flags
 */

#define STR_LEFT    02		/* implicit left anchor     */
#define STR_RIGHT   04		/* implicit right anchor    */

#define CC_bel      0007	/* bel character        */
#define CC_esc      0033	/* esc character        */
#define CC_vt       0013	/* vt character         */

#define elementsof(x)   (sizeof(x)/sizeof(x[0]))

    extern char *pathpath(const char *);
    extern char *pathfind(const char *, const char *, const char *);
    extern char *pathaccess(const char*, const char*, const char*);
    extern const char *pathcat(agxbuf *, const char *, const char *,
			 const char *);

    extern int chresc(const char *, char **);
    extern int chrtoi(const char *);
    extern char *fmtesq(const char *, const char *);
    extern char *fmtesc(const char *as);
    extern char *fmtbuf(size_t n);
    extern char *fmtquote(const char*, const char*, const char*, size_t);

    extern int strmatch(char *, char *);
    extern int strgrpmatch(char *, char *, int *, int, int);
    extern void stresc(char *);

#ifdef __cplusplus
}
#endif
