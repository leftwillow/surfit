
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

#include "flow_ie.h"

#include "fileio.h"

#include "prod_area.h"
#include "area.h"
#include "vec.h"
#include "grid_line.h"
#include "bitvec.h"
#include "free_elements.h"

#include "grid_user.h"
#include "grid_line_user.h"

namespace surfit {

f_prod_area::f_prod_area(REAL ivalue, const d_area * iarea) :
functional("f_prod_area") 
{
	area = iarea;
	value = ivalue;
	if (area->getName()) {
		setNameF("f_approx_area %s", area->getName());
	}
};

f_prod_area::~f_prod_area() {
};

int f_prod_area::this_get_data_count() const {
	return 1;
};

const data * f_prod_area::this_get_data(int pos) const {
	if (pos == 0)
		return area;
	return NULL;
};

bool f_prod_area::minimize() {
	return false;
};

bool f_prod_area::make_matrix_and_vector(matr *& matrix, vec *& v) {

	if (area->getName())
		writelog(LOG_MESSAGE,"production area (%s), value = %g", area->getName(), value);
	else 
		writelog(LOG_MESSAGE,"production area noname, value = %g", value);
	
	int matrix_size = method_basis_cntX*method_basis_cntY;
	v = create_vec(matrix_size);

	bitvec * mask = NULL;
	mask = nodes_in_area_mask(area, method_grid, method_mask_undefined);
	if (!mask)
		return false;

	int true_size = mask->true_size();
		
	int i;
	int points = 0;

	for (i = 0; i < matrix_size; i++) {
		
		if ( (method_mask_solved->get(i)) ) {
			mask->set_false(i);
			continue;
		}

		if (mask->get(i)) {
			(*v)(i) = value/(REAL)true_size;
			points++;
		}
	}

	mask->release();
	
	matrix = NULL;

	bool solvable = (points > 0);

	solvable = wrap_sums(matrix, v) || solvable;
	
	return solvable;
};

void f_prod_area::mark_solved_and_undefined(bitvec * mask_solved, bitvec * mask_undefined, bool i_am_cond) {
	if ((functionals_add->size() == 0) && ( !cond() ) && (i_am_cond == false) )
		return;	

	mark_sums(mask_solved, mask_undefined);
};

bool f_prod_area::solvable_without_cond(const bitvec * mask_solved,
					const bitvec * mask_undefined,
					const vec * X)
{

	int i;
	for (i = 0; i < mask_solved->size(); i++) {
		if (mask_solved->get(i))
			goto sss;
	}

	return false;

sss:
	
	return true;

};


}; // namespace surfit;

