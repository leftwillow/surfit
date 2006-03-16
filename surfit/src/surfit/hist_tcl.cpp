
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

#include "datafile.h"
#include "fileio.h"
#include "sort_alg.h"
#include "vec.h"
#include "stepFunc.h"

#include "hist.h"
#include "hist_tcl.h"
#include "hist_internal.h"
#include "variables_internal.h"
#include "free_elements.h"
#include "func.h"

#include <math.h>
#include <float.h>

namespace surfit {

bool hist_read(const char * filename, const char * histname,
	       int col1, int col2, int col3,
	       const char* delimiter, int skip_lines, int grow_by)
{
	d_hist * hst = _hist_read(filename, histname,
				  col1, col2, col3, skip_lines,
				  grow_by, delimiter);

	if (hst != NULL) {
		surfit_hists->push_back(hst);
		return true;
	}
	return false;

};

bool hist_write(const char * filename, const char * delimiter, const char * pos) 
{
	d_hist * hst = get_element<d_hist>(pos, surfit_hists->begin(), surfit_hists->end());
	if (hst == NULL)
		return false;

	char buf[80];
	strcpy( buf, "%lf" );
	strcat( buf, delimiter );
	strcat( buf, "%lf" );
	strcat( buf, delimiter );
	strcat( buf, "%lf\n" );
	return _hist_write(hst, filename, buf);
};

bool hist_save(const char * filename, const char * pos) {
	d_hist * hst = get_element<d_hist>(pos, surfit_hists->begin(), surfit_hists->end());
	if (hst == NULL)
		return false;

	writelog(LOG_MESSAGE,"Saving hist to file %s", filename);

	return _hist_save(hst, filename);
};

bool hist_load(const char * filename, const char * histname) {
	d_hist * hst = _hist_load(filename, histname);
	if (hst) {
		surfit_hists->push_back(hst);
		return false;
	}
	return true;
};

bool hist_setName(const char * new_name, const char * pos) {
	d_hist * hst = get_element<d_hist>(pos, surfit_hists->begin(), surfit_hists->end());
	if (hst == NULL)
		return false;
	hst->setName(new_name);
	return true;
};

const char * hist_getName(const char * pos) {
	d_hist * hst = get_element<d_hist>(pos, surfit_hists->begin(), surfit_hists->end());
	if (hst == NULL)
		return false;
	return hst->getName();
};

bool hist_delall() {
	if (surfit_hists->size() == 0) {
		//writelog(SURFIT_WARNING,"hists_delall : empty surfit_hists");
		return false;
	}

	release_elements(surfit_hists->begin(), surfit_hists->end());
	surfit_hists->resize(0);
	return true;
};

bool hist_del(const char * pos) {
	std::vector<d_hist *>::iterator hst = get_iterator<d_hist>(pos, surfit_hists->begin(), surfit_hists->end());
	if (hst == surfit_hists->end())
		return false;
	if (*hst)
		(*hst)->release();
	surfit_hists->erase(hst);
	return true;
};

int hist_size() {
	return surfit_hists->size();
};

void hists_info() {
	unsigned int hists_counter;
	for (hists_counter = 0; hists_counter < surfit_hists->size(); hists_counter++) {
		d_hist * a_hist = *(surfit_hists->begin()+hists_counter);
		_hist_info(a_hist);
	}
};

bool hist_from_func(const char * histname, const char * func_pos, int intervs) {
	d_func * fnc = get_element<d_func>(func_pos, surfit_funcs->begin(), surfit_funcs->end());
	if (fnc == NULL)
		return false;

	REAL minz, maxz;
	fnc->getMinMaxZ(minz, maxz);
	maxz += (maxz-minz)*1e-3;
	REAL step = stepFunc(minz, maxz, intervs);
	minz = floor(minz/step)*step;
	maxz = floor(maxz/step+1)*step;
	intervs++;

	vec * X1 = create_vec(intervs);
	vec * X2 = create_vec(intervs);
	int i;
	for (i = 0; i < intervs; i++) {
		(*X1)(i) = minz + i*step;
		(*X2)(i) = minz + (i+1)*step;
	}

	vec * Z = create_vec(intervs);
	
	for (i = 0; i < intervs; i++) 
		(*Z)(i) = 0;

	int fnc_size = 0;

	for (i = 0; i < fnc->coeff->size(); i++) {
		REAL z = (*(fnc->coeff))(i);
		if (z == fnc->undef_value)
			continue;
		fnc_size += 1;
		int pos = (int)floor((z-minz)/step + 0.5);
		(*Z)(pos) += 1;
	}

	for (i = 0; i < intervs; i++) 
		(*Z)(i) /= (REAL)(fnc_size);

	d_hist * hst = create_hist(X1, X2, Z, histname);
	surfit_hists->push_back(hst);

	return true;
};

bool hist_update_func(const char * hist_pos, const char * func_pos) {

	d_hist * hst = get_element<d_hist>(hist_pos, surfit_hists->begin(), surfit_hists->end());
	if (hst == NULL)
		return false;

	d_func * fnc = get_element<d_func>(func_pos, surfit_funcs->begin(), surfit_funcs->end());
	if (fnc == NULL)
		return false;

	int hist_size = hst->size();
	int fnc_size = 0;
	int i, j;
	
	for (i = 0; i < hist_size - 1; i++) 
		(*(hst->Z))(i) = 0;

	REAL from, to, z;
	
	for (j = 0; j < fnc->coeff->size(); j++) {
		z = (*(fnc->coeff))(j);
		if (z == fnc->undef_value)
			continue;
		fnc_size ++;
		for (i = 0; i < hist_size - 1; i++) {
			from = (*(hst->X1))(i);
			to   = (*(hst->X2))(i);
			if ((z >= from) && (z < to))
				(*(hst->Z))(i) += 1;
		}
	}

	for (i = 0; i < hist_size; i++) 
		(*(hst->Z))(i) /= (REAL)(fnc_size);
	
	return true;

};

}; // namespace surfit;

