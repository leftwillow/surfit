
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

#ifndef __surfit_wiso_included__
#define __surfit_wiso_included__

#include "iso.h"

namespace surfit {

/*! \class wiso
    \brief line with a constant value (isoline) and informational weight (see \ref surfit_wiso). See \ref tcl_wiso "Tcl commands" for \ref wiso.
*/
class SURFIT_EXPORT wiso : public iso {
public:
	/*! constructor
	    \param iX_begin pointer to the first element of X-coordinates vector
	    \param iX_end   pointer to the last+1 element of X-coordinates vector
	    \param iY_begin pointer to the first element of Y-coordinates vector
	    \param iY_end   pointer to the last+1 element of Y-coordinates vector
	    \param ivalue   value for isoline,
	    \param iweight  informational weight
	    \param iso_name name of isoline
	*/
	wiso(REAL * iX_begin, REAL * iX_end,
	     REAL * iY_begin, REAL * iY_end,
	     REAL ivalue, REAL iweight,
	     const char * iso_name = NULL);

	//! constructor
	wiso(curv * crv, REAL ivalue, REAL iweight);

	//! constructor
	wiso(iso * crv, REAL iweight);

	//! destructor
	virtual ~wiso();

	//! informational weight for isoline
	REAL weight;

};

}; // namespace surfit;

#endif

