
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

#include "vec.h"
#include "vec_alg.h"

#include "hist.h"
#include "free_elements.h"

namespace surfit {

d_hist * create_hist(REAL ifrom, REAL ito,
		     vec * iZ,
		     const char * hist_name)
{
	return new d_hist(ifrom, ito, iZ, hist_name);
};

d_hist::d_hist(REAL from, REAL to,
	       vec * iZ, 
	       const char * iname) : data("d_hist")
{
	z_from = from;
	z_to = to;
	cnt = iZ->size();
	z_step = (z_to-z_from)/REAL(cnt);
	
	hst = iZ;

	setName(iname);
};

d_hist::~d_hist() {
	if (hst)
		hst->release();
};

size_t d_hist::operator()(REAL value) const 
{
	if (value < z_from)
		return UINT_MAX;
	if (value > z_to)
		return UINT_MAX;

	size_t pos = (value-z_from)/z_step;
	return pos;
};

REAL d_hist::operator ()(size_t pos) const
{
	return (*hst)(pos);
};

bool d_hist::get_interv(size_t pos, REAL & value_from, REAL & value_to, REAL & hst_value) const
{
	if (pos > size())
		return false;
	value_from = z_from + pos*z_step;
	value_to = value_from + z_step;
	hst_value = (*hst)(pos);
	return true;
};

bool d_hist::bounds(REAL & minx, REAL & maxx, REAL & miny, REAL & maxy) const 
{
	return false;
};
	
bool d_hist::getMinMaxZ(REAL & minz, REAL & maxz) const 
{
	return false;
};

size_t d_hist::size() const {
	return cnt;
};

REAL d_hist::get_step() const {
	return z_step;
};

std::vector<d_hist *>     * surfit_hists     = NULL;

/*! \struct hist_garbage
    \brief struct for deletion of \ref hist pointers
*/
struct hist_garbage : public binman {
	//! inits \ref surfit_hist and \ref surfit_hists
	hist_garbage() {
		surfit_hists = new std::vector<d_hist *>;
	}
	//! removes \ref surfit_hist and \ref surfit_hists
	~hist_garbage() {
		if (surfit_hists) {
			release_elements(surfit_hists->begin(), surfit_hists->end());
			delete surfit_hists;
		}
	}

};

hist_garbage hist_garb;

}; // namespace surfit;

