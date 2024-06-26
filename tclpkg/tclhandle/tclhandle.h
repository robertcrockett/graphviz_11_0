/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <stdint.h>
#include <tcl.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Macro to rounded up a size to be a multiple of (void *).  This is required
 * for systems that have alignment restrictions on pointers and data.
 */
#define ROUND_ENTRY_SIZE(size)                                                 \
  ((((size) + tclhandleEntryAlignment - 1) / tclhandleEntryAlignment) *        \
   tclhandleEntryAlignment)

#define NULL_IDX UINT64_MAX
#define ALLOCATED_IDX (UINT64_MAX - 1)

typedef unsigned char ubyte_t;
typedef ubyte_t *ubyte_pt;

/*
 * This is the table header.  It is separately allocated from the table body,
 * since it must keep track of a table body that might move.  Each entry in the
 * table is preceded with a header which has the free list link, which is a
 * entry index of the next free entry.  Special values keep track of allocated
 * table is preceded with a header which has the free list link, which is a
 * entry index of the next free entry.  Special values keep track of allocated
 * entries.
 */

typedef struct {
  uint64_t entrySize;   /* Entry size in bytes, including overhead */
  uint64_t tableSize;   /* Current number of entries in the table  */
  uint64_t freeHeadIdx; /* Index of first free entry in the table  */
  char *handleFormat;   /* Malloc'ed copy of prefix string + "%lu" */
  ubyte_pt bodyPtr;     /* Pointer to table body                   */
} tblHeader_t;
typedef tblHeader_t *tblHeader_pt;

typedef struct {
  uint64_t freeLink;
} entryHeader_t;
typedef entryHeader_t *entryHeader_pt;

#define ENTRY_HEADER_SIZE (ROUND_ENTRY_SIZE(sizeof(entryHeader_t)))

/*
 * This macro is used to return a pointer to an entry, given its index.
 */
#define TBL_INDEX(hdrPtr, idx)                                                 \
  ((entryHeader_pt)(hdrPtr->bodyPtr + (hdrPtr->entrySize * idx)))

/*
 * This macros to convert between pointers to the user and header area of
 * an table entry.
 */
#define USER_AREA(entryPtr) (void *)(((ubyte_pt)entryPtr) + ENTRY_HEADER_SIZE);
#define HEADER_AREA(entryPtr)                                                  \
  (entryHeader_pt)(((ubyte_pt)entryPtr) - ENTRY_HEADER_SIZE);

void *tclhandleFreeIndex(tblHeader_pt headerPtr, uint64_t entryIdx);
void *tclhandleFree(tblHeader_pt headerPtr, char *handle);
tblHeader_pt tclhandleInit(char *prefix, uint64_t entrySize,
                           uint64_t initEntries);
void *tclhandleXlateIndex(tblHeader_pt headerPtr, uint64_t entryIdx);
void *tclhandleXlate(tblHeader_pt headerPtr, char *handle);
entryHeader_pt tclhandleAlloc(tblHeader_pt tblHdrPtr, char **handle,
                              uint64_t *entryIdxPtr);
int tclhandleIndex(tblHeader_pt tblHdrPtr, char *handle, uint64_t *entryIdxPtr);

#ifdef __cplusplus
}
#endif
