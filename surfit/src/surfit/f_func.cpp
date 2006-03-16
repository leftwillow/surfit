
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

#include "surfit_ie.h"
#include "f_func.h"
#include "func.h"
#include "solvers.h"
#include "variables_tcl.h"
#include "matr.h"
#include "vec.h"
#include "bitvec.h"
#include "grid.h"
#include "matr_eye.h"
#include "grid_internal.h"

#include "grid_user.h"

namespace surfit {

f_func::f_func(const d_func * ifnc) :
functional("f_func")
{
	fnc = ifnc;
	if (fnc->getName()) {
		setNameF("f_func %s", fnc->getName());
	}
	mask = NULL;
};

f_func::~f_func() {
	if (mask)
		mask->release();
};

int f_func::this_get_data_count() const {
	return 1;
};

const data * f_func::this_get_data(int pos) const {
	if (pos == 0)
		return fnc;
	return false;
};

bool f_func::minimize() {
	if ((functionals_add->size() == 0) && ( !cond() )) {
		return minimize_only_func();
	} else {
		
		matr * A = NULL;
		vec * b = NULL;
		bool solvable = make_matrix_and_vector(A,b);
		
		int matrix_size = method_basis_cntX*method_basis_cntY;
		
		if ( !cond() ) {
			if (solvable == false) {
				delete A;
				if (b)
					b->release();
				return false;
			}
			
			method_X->resize(b->size());
			solve(A,b,method_X);
			method_X->resize(matrix_size);
			
			delete A;
			if (b)
				b->release();
			
			return true;
		} else {
			return solve_with_penalties(this, A, b, method_X);
		}
		return false;
		
	}
	return false;
};

bool f_func::make_matrix_and_vector(matr *& matrix, vec *& v) {

	if (fnc->getName())
		writelog(LOG_MESSAGE,"func : (%s), size=(%d x %d)", fnc->getName(), fnc->getCountX(), fnc->getCountY());
	else 
		writelog(LOG_MESSAGE,"func : noname, size=(%d x %d)", fnc->getCountX(), fnc->getCountY());

	int NN = method_grid->getCountX();
	int MM = method_grid->getCountY();

	v = create_vec(NN*MM);
	
	if (mask)
		mask->release();
	mask = create_bitvec(NN*MM);
	mask->init_false();
	int points = 0;

	int from_x, from_y, to_x, to_y;
	_grid_intersect1(method_grid, fnc->grd,
		        from_x, to_x,
		        from_y, to_y);

	int i, j, pos;
	
	REAL value;
	REAL stepX2 = method_grid->stepX/REAL(2);
	REAL stepY2 = method_grid->stepY/REAL(2);

	for (j = from_y; j <= to_y; j++) {
		for (i = from_x; i <= to_x; i++) {
			
			pos = i + j * method_grid->getCountX();

			// check for existance
			if (method_mask_solved->get(pos))
				continue;
			if (method_mask_undefined->get(pos))
				continue;

			REAL x, y;
			method_grid->getCoordNode(i, j, x, y);

			//value = fnc->getMeanValue(x-stepX2, x+stepX2, y-stepY2, y+stepY2);
			value = fnc->getInterpValue(x, y);

			if (value == fnc->undef_value)
				continue;

			(*v)(pos) = value;
			mask->set_true(pos);
			points++;
			
		}
	}

	matr_eye * T = new matr_eye(1, NN*MM, mask, method_mask_solved, method_mask_undefined);
	matrix = T;

	bool solvable = (points > 0);

	solvable = wrap_sums(matrix, v) || solvable;
	
	return solvable;

};
	
void f_func::mark_solved_and_undefined(bitvec * mask_solved, bitvec * mask_undefined, bool i_am_cond) {
	if ((functionals_add->size() == 0) && ( !cond() ) && (i_am_cond == false))
		return;

	int i;
	if (mask) {
		for (i = 0; i < mask->size(); i++) {
			if (mask->get(i))
				mask_solved->set_true(i);
		}
	}
};

bool f_func::minimize_only_func() {

	if (fnc->getName())
		writelog(LOG_MESSAGE,"func : (%s), size=(%d x %d)", fnc->getName(), fnc->getCountX(), fnc->getCountY());
	else 
		writelog(LOG_MESSAGE,"func : noname, size=(%d x %d)", fnc->getCountX(), fnc->getCountY());

	int from_x, from_y, to_x, to_y;
	_grid_intersect1(method_grid, fnc->grd,
		        from_x, to_x,
		        from_y, to_y);

	int i, j, pos;
	
	REAL value;
	REAL stepX2 = method_grid->stepX/REAL(2);
	REAL stepY2 = method_grid->stepY/REAL(2);

	for (j = from_y; j <= to_y; j++) {
		for (i = from_x; i <= to_x; i++) {
			
			pos = i + j * method_grid->getCountX();

			// check for existance
			if (method_mask_solved->get(pos))
				continue;
			if (method_mask_undefined->get(pos))
				continue;

			REAL x, y;
			method_grid->getCoordNode(i, j, x, y);

			//value = fnc->getMeanValue(x-stepX2, x+stepX2, y-stepY2, y+stepY2);
			value = fnc->getInterpValue(x, y);

			if (value == fnc->undef_value)
				continue;

			(*method_X)(pos) = value;
			method_mask_solved->set_true(pos);
			
		}
	}
	
	return true;
};

bool f_func::solvable_without_cond(const bitvec * mask_solved,
		      const bitvec * mask_undefined,
		      const vec * X)
{
	return true;
};

}; // namespace surfit;

