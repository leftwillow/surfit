
/*------------------------------------------------------------------------------
 *	$Id$
 *
 *	Copyright (c) 2002-2004 by M. V. Dmitrievsky and V. N. Kutrunov
 *	See COPYING file for copying and redistribution conditions.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	Contact info: surfit.sourceforge.net
 *----------------------------------------------------------------------------*/

#ifndef __surfit_flow_func_internal_included__
#define __surfit_flow_func_internal_included__

namespace surfit {

class func;

FLOW_EXPORT
/*!  \ingroup internal_pfunc
     \fn REAL _func_debit(func * fnc, REAL x, REAL y);
     \brief calculates debit at point (x,y), using four neighbour cells
*/
REAL _func_debit(func * fnc, REAL x, REAL y);

FLOW_EXPORT
/*! \ingroup internal_pfunc
    \fn REAL _func_debit_rect(func * fnc, REAL x1, REAL y1, REAL x2, REAL y2);
    \brief calculates debit in rect
*/
REAL _func_debit_rect(func * fnc, REAL x1, REAL y1, REAL x2, REAL y2);

}; // namespace surfit;

#endif

