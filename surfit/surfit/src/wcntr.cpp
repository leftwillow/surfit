
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

#include "ie.h"
#include "wcntr.h"

namespace surfit {

wcntr::wcntr(REAL * iX_begin, REAL * iX_end,
	     REAL * iY_begin, REAL * iY_end,
	     REAL * iZ_begin, REAL * iZ_end,
	     REAL iweight,
	     const char * cntr_name) : 
cntr(iX_begin, iX_end,
     iY_begin, iY_end,
     iZ_begin, iZ_end,
     cntr_name) 
{
	weight = iweight;
};

wcntr::~wcntr() {};

}; // namespace surfit;


