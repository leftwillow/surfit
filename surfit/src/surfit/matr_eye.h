
/*------------------------------------------------------------------------------
 *	$Id$
 *
 *	Copyright (c) 2002-2006 by M. V. Dmitrievsky and V. N. Kutrunov
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

#ifndef __surfit_matr_eye__
#define __surfit_matr_eye__

#include "matr.h"

namespace surfit {

class bitvec;

class SURFIT_EXPORT matr_eye : public matr {
public:
	//! constructor
	matr_eye(REAL ival, long iN, bitvec * imask,
		  const bitvec * imask_solved, const bitvec * imask_undefined);

	//! A destructor
	virtual ~matr_eye();

	REAL element_at(int i, int j, int * next_j = NULL) const;
	REAL element_at_transposed(int i, int j, int * next_j = NULL) const;
	REAL at(int i, int j, int * next_j = NULL) const;
	REAL at_transposed(int i, int j, int * next_j = NULL) const;
	
	REAL mult_line(int J, const REAL * b_begin, const REAL * b_end);
	REAL mult_transposed_line(int J, const REAL * b_begin, const REAL * b_end);
	
	virtual long cols() const;
	virtual long rows() const;

	REAL norm() const;

protected:

	long N;
	REAL val;
	bitvec * mask;
	const bitvec * mask_solved;
	const bitvec * mask_undefined;

};


}; // namespace surfit;

#endif


