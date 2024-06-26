/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

/*
 * Glenn Fowler
 * AT&T Research
 *
 * expression library
 */

#include <expr/exlib.h>
#include <stddef.h>

/*
 * return the expression for name or sym coerced to type
 */

Exnode_t*
exexpr(Expr_t* ex, const char* name, Exid_t* sym, int type)
{
	if (ex)
	{
		if (!sym)
			sym = name ? dtmatch(ex->symbols, name) : &ex->main;
		if (sym && sym->lex == PROCEDURE && sym->value)
		{
			if (type != DELETE_T)
				return excast(ex, sym->value->data.procedure.body, type, NULL, 0);
			exfreenode(ex, sym->value);
			sym->lex = NAME;
			sym->value = 0;
		}
	}
	return 0;
}

/**
 * @dir lib/expr
 * @brief expression library for lib/gvpr/, API expr.h
 */
