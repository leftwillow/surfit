
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

#ifndef __surfit_approx_surf_included__
#define __surfit_approx_surf_included__

#include "functional.h"

namespace surfit {

class d_surf;
class matr;
class vec;
class bitvec;

/*! \class f_surf
    \brief Functional for approximation surface with other surface values
*/
class SURFIT_EXPORT f_surf : public functional {
public:
	//! constructor
	f_surf(const d_surf * isrf);

	//! destructor
	~f_surf();

	bool minimize();

	bool make_matrix_and_vector(matr *& matrix, vec *& v);

	bool solvable_without_cond(const bitvec * mask_solved, 
				   const bitvec * mask_undefined,
				   const vec * X);

	void mark_solved_and_undefined(bitvec * mask_solved, 
				       bitvec * mask_undefined,
				       bool i_am_cond);
	
protected:

	int this_get_data_count() const;
	const data * this_get_data(int pos) const;

	//! fast minimization procedure
	bool minimize_only_surf();

	//! data for functional
	const d_surf * srf;

	//! mask for functional
	bitvec * mask;
	
};

}; // namespace surfit;

#endif
