
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

#ifndef __surfit_f_area_func_ineq_included__
#define __surfit_f_area_func_ineq_included__

#include "functional.h"

namespace surfit {

class d_area;
class d_func;

/*! \class f_area_func_ineq
    \brief functional that sets inequality condition for area (with function)
*/
class SURFIT_EXPORT f_area_func_ineq : public functional {
public:

	//! constructor
	f_area_func_ineq(const d_func * ifnc, const d_area * iarea, bool ileq, REAL imult, bool iinside = true);
	//! destructor
	~f_area_func_ineq();

	bool minimize();

	bool make_matrix_and_vector(matr *& matrix, vec *& v);

	bool solvable_without_cond(const bitvec * mask_solved, 
				   const bitvec * mask_undefined,
				   const vec * X);

	void mark_solved_and_undefined(bitvec * mask_solved, 
				       bitvec * mask_undefined,
				       bool i_am_cond);

	void drop_private_data();
	
protected:

	int this_get_data_count() const;
	const data * this_get_data(int pos) const;

private:

	//! calculates mask for area
	void get_area_mask();

	//! area, that defines region
	const d_area * area;

	//! function for equation
	const d_func * fnc;

	//! equation flag
	bool leq;

	//! parameter for penalty algorithm
	REAL mult;

	//! area is inside or outside region
	bool inside;

	//! mask for area
	bitvec * area_mask;

};

}; // namespace surfit

#endif

