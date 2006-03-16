
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

#include "mask.h"
#include "func.h"
#include "mask_internal.h"
#include "mask_tcl.h"
#include "func_internal.h"
#include "func_tcl.h"
#include "grid.h"
#include "bitvec.h"
#include "variables_internal.h"

#include <errno.h>
#include <float.h>

namespace surfit {

bool mask_load(const char * filename, const char * defname) {
	d_mask * msk = _mask_load(filename, defname);
	if (msk) {
		surfit_masks->push_back(msk);
		return true;
	}
	return false;
};

bool mask_save(const char * filename, const char * pos) {
	d_mask * msk = get_element<d_mask>(pos, surfit_masks->begin(), surfit_masks->end());
	if (!msk)
		return false;

	return _mask_save(msk, filename);
};

bool mask_save_grd(const char * filename, const char * pos) {

	d_mask * msk = get_element<d_mask>(pos, surfit_masks->begin(), surfit_masks->end());
	if (!msk)
		return false;

	if (!filename)
		return false;

	FILE * f = fopen(filename,"w");

	if (!f) {
		writelog(LOG_ERROR, "Can't write data to file %s",filename,strerror( errno ));
		return false;
	}

	writelog(LOG_MESSAGE,"Saving mask %s to file %s (grd-ASCII)", msk->getName(), filename);

	fprintf(f,"DSAA\n");
	int nx = msk->getCountX();
	int ny = msk->getCountY();

	fprintf(f,"%d %d\n", nx, ny);
	fprintf(f,"%lf %lf\n", msk->getMinX(), msk->getMaxX());
	fprintf(f,"%lf %lf\n", msk->getMinY(), msk->getMaxY());
	fprintf(f,"0 1\n");

	// matrix 
	int iy, ix;
	int ncnt;
	int cnt = 0;
	REAL val;
	
	for(iy=0; iy<ny; iy++)	{
		ncnt = 0;
		
		for(ix=0; ix<nx; ix++)	{
			val = msk->coeff->get( ix + nx*iy );
			fprintf(f,"%lf ", val);
			if (ncnt>9) { 
				fprintf(f,"\n");
				ncnt = 0;
			}
			ncnt++;
		}
		fprintf(f,"\n");
	}

	fclose(f);

	return true;
};

bool mask_save_xyz(const char * filename, const char * pos) {

	d_mask * msk = get_element<d_mask>(pos, surfit_masks->begin(), surfit_masks->end());
	if (!msk)
		return false;

	if (!filename)
		return false;

	FILE * f = fopen(filename,"w");

	if (!f) {
		writelog(LOG_ERROR, "Can't write data to file %s",filename,strerror( errno ));
		return false;
	}

	writelog(LOG_MESSAGE,"Saving mask %s to file %s (xyz-ASCII)", msk->getName(), filename);

	int nx = msk->getCountX();
	int ny = msk->getCountY();

	int iy, ix;
	int cnt = 0;
	REAL val;
	REAL x_coord, y_coord;
	
    
	for(iy=0; iy<ny; iy++)	{
		for(ix=0; ix<nx; ix++)	{
			msk->getCoordNode(ix, iy, x_coord, y_coord);
			val = msk->coeff->get( ix + nx*iy );
			fprintf(f,"%lf %lf %lf \n", x_coord, y_coord, val);
			
		}
	}

	fclose(f);

	return true;
};

bool mask_or(const char * pos1, const char * pos2) {

	d_mask * def1 = get_element<d_mask>(pos1, surfit_masks->begin(), surfit_masks->end());
	if (!def1)
		return false;

	d_mask * def2 = get_element<d_mask>(pos2, surfit_masks->begin(), surfit_masks->end());
	if (!def2)
		return false;

	writelog(LOG_MESSAGE,"mask : OR operation");
	
	def1->OR(def2);
	return true;
};

bool mask_xor(const char * pos1, const char * pos2) {

	d_mask * def1 = get_element<d_mask>(pos1, surfit_masks->begin(), surfit_masks->end());
	if (!def1)
		return false;

	d_mask * def2 = get_element<d_mask>(pos2, surfit_masks->begin(), surfit_masks->end());
	if (!def2)
		return false;

	writelog(LOG_MESSAGE,"mask : XOR operation");
	def1->XOR(def2);
	return true;
};

bool mask_and(const char * pos1, const char * pos2) {

	d_mask * def1 = get_element<d_mask>(pos1, surfit_masks->begin(), surfit_masks->end());
	if (!def1)
		return false;

	d_mask * def2 = get_element<d_mask>(pos2, surfit_masks->begin(), surfit_masks->end());
	if (!def2)
		return false;

	writelog(LOG_MESSAGE,"mask : AND operation");
	def1->AND(def2);
	return true;
};

bool mask_not(const char * pos1, const char * pos2) {

	d_mask * def1 = get_element<d_mask>(pos1, surfit_masks->begin(), surfit_masks->end());
	if (!def1)
		return false;

	d_mask * def2 = get_element<d_mask>(pos2, surfit_masks->begin(), surfit_masks->end());
	if (!def2)
		return false;

	writelog(LOG_MESSAGE,"mask : NOT operation");
	
	def1->NOT(def2);
	return true;
};

bool mask_by_func(const char * func_pos) {
	
	d_func * fnc = get_element<d_func>(func_pos, surfit_funcs->begin(), surfit_funcs->end());
	if (!fnc)
		return false;

	d_mask * msk = _mask_by_func(fnc);
	if (msk) {
		surfit_masks->push_back(msk);
		return true;
	}
	return false;
};

bool mask_apply_to_func(const char * def_pos, const char * func_pos) {

	d_mask * msk = get_element<d_mask>(def_pos, surfit_masks->begin(), surfit_masks->end());
	if (!msk)
		return false;

	d_func * fnc = get_element<d_func>(func_pos, surfit_funcs->begin(), surfit_funcs->end());
	if (!fnc)
		return false;

	return _mask_apply_to_func(msk, fnc);
};

bool mask_setName(const char * new_name, const char * pos) {

	d_mask * msk = get_element<d_mask>(pos, surfit_masks->begin(), surfit_masks->end());
	if (!msk)
		return false;
	msk->setName(new_name);
	return true;
};

const char * mask_getName(const char * pos) {
	d_mask * msk = get_element<d_mask>(pos, surfit_masks->begin(), surfit_masks->end());
	if (!msk)
		return false;
	return msk->getName();
};

bool mask_getValue(REAL x, REAL y, const char * pos) {
	d_mask * msk = get_element<d_mask>(pos, surfit_masks->begin(), surfit_masks->end());
	if (!msk)
		return false;
	return msk->getValue(x,y);
};

bool mask_to_func(const char * pos) {
	d_mask * msk = get_element<d_mask>(pos, surfit_masks->begin(), surfit_masks->end());
	std::vector<d_mask *>::iterator pdef = get_iterator<d_mask>(pos, surfit_masks->begin(), surfit_masks->end());
	if (!msk)
		return false;

	d_func * fnc = create_func_by_mask(msk);
	if (msk)
		msk->release();
	surfit_masks->erase(pdef);
	surfit_funcs->push_back(fnc);
	return true;
};

bool mask_delall() {
	unsigned int i;

	if (surfit_masks->size() == 0) {
		//writelog(SURFIT_WARNING,"masks_delall : empty surfit_masks");
		return false;
	}
	
	for (i = 0; i < surfit_masks->size(); i++) {
		d_mask * msk = (*surfit_masks)[i];
		if (msk)
			msk->release();
	}
	
	surfit_masks->resize(0);

	return true;
};

bool mask_del(const char * pos) {
	std::vector<d_mask *>::iterator msk = get_iterator<d_mask>(pos, surfit_masks->begin(), surfit_masks->end());
	if (msk == surfit_masks->end())
		return false;
	if (*msk)
		(*msk)->release();
	surfit_masks->erase(msk);
	return true;
};

void masks_info() {
	if (surfit_masks->size() > 0) {
		unsigned int mask_cnt;
		for (mask_cnt = 0; mask_cnt < surfit_masks->size(); mask_cnt++) {
			d_mask * a_fnc = *(surfit_masks->begin()+mask_cnt);
			if (a_fnc) {
				_mask_info(a_fnc);
			}
		}
	}
};

int mask_size() {
	return surfit_masks->size();
};

void surfit_mask_add(d_mask * msk) {
	surfit_masks->push_back(msk);
};

}; // namespace surfit;
