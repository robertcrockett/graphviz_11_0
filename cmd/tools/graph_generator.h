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

typedef void (*edgefn)(int, int);

extern void makeBall(int, int, edgefn);
extern void makeCircle(int, edgefn);
extern void makeComplete(int, edgefn);
extern void makeCompleteB(int, int, edgefn);
extern void makePath(int, edgefn);
extern void makeStar(int, edgefn);
extern void makeWheel (int, edgefn);
extern void makeTorus(int, int, edgefn);
extern void makeTwistedTorus(int, int, int, int, edgefn);
extern void makeCylinder(int, int, edgefn);
extern void makeRandom(int, int, edgefn);
extern void makeSquareGrid(int, int, int, int, edgefn);
extern void makeBinaryTree(int, edgefn);
extern void makeSierpinski(int, edgefn);
extern void makeTetrix(int, edgefn);
extern void makeHypercube(int, edgefn);
extern void makeTree(int, int, edgefn);
extern void makeTriMesh(int, edgefn);
extern void makeMobius(int, int, edgefn);

typedef struct treegen_s treegen_t;
extern treegen_t* makeTreeGen (int);
extern void makeRandomTree (treegen_t*, edgefn);
extern void freeTreeGen(treegen_t*);
