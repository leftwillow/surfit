
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

#include "sstuff.h"
#include "fileio.h"
#include "datafile.h"
#include "interp.h"

#include "data_manager.h"

#include <string.h>
#include <float.h>
#include <tcl.h>

#include "grid_internal.h"
#include "grid.h"
#include "grid_tcl.h"
#include "grid_user.h"
#include "func.h"
#include "func_internal.h"
#include "func_tcl.h"
#include "mask.h"
#include "mask_internal.h"
#include "mask_tcl.h"
#include "points.h"
#include "pnts_internal.h"
#include "pnts_tcl.h"
#include "cntr.h"
#include "cntr_internal.h"
#include "cntr_tcl.h"
#include "curv.h"
#include "curv_internal.h"
#include "curv_tcl.h"
#include "area.h"
#include "area_internal.h"
#include "area_tcl.h"
#include "grid_line_tcl.h"
#include "grid_line_user.h"
#include "sort_alg.h"
#include "free_elements.h"
#include "variables.h"
#include "variables_internal.h"
#include "variables_tcl.h"
#include "functional.h"
#include "cmofs.h"

namespace surfit {

char * file_info(const char * filename) {

	datafile *df = new datafile(filename, 1);
	
	if (!df->condition()) {
		delete df;
		return NULL;
	}

	char * contents = NULL;
	df->file_info(contents);
	delete df;
	if (contents) {
		char * res = strdup(contents);
		sstuff_free_char(contents);
		return res;
	}

	free(contents);
	return NULL;
	
};

//////////////////////////////
//
// data_manager
//
//////////////////////////////

data_manager::data_manager() {
	managers = new std::vector<manager*>;
	managers->reserve(1000);
};

data_manager::~data_manager() {
	if (managers) {
		release_elements(managers->begin(), managers->end());
	}
	delete managers;
};

bool data_manager::auto_load(const char * filename, const char * first1024, int readed) const {

	bool res = false;

	if (managers) {
		std::vector<manager *>::reverse_iterator it;
		for (it = managers->rbegin(); it != managers->rend(); it++) {
			res = (*it)->auto_load(filename, first1024, readed);
			if (res)
				return true;
		}
	}
	
	return false;
};


bool data_manager::load(datafile * df) const {

	char tagname[TAG_WORD_SIZE];
	if ( !df->readTagName(tagname) ) 
		return false;

	int res = 0;
	while ( strcmp(tagname,"eof") != 0 ) {
		
		if (managers) {
			std::vector<manager *>::iterator it;
			for (it = managers->begin(); it != managers->end(); it++) {
				res = (*it)->load_tag(df, tagname);
				if (res == -1)
					return false;
				if (res == 1)
					break;
			}
		}
		
		if (res == 0) {
			df->skipTag(false);
			
			if ( !df->readTagName(tagname) ) 
				return false;
		}
		
	};

	return true;
};

void data_manager::clear_data() const {
	
	if (managers) {
		std::vector<manager *>::iterator it;
		for (it = managers->begin(); it != managers->end(); it++) 
			if (*it)
				(*it)->clear_data();
	}
	
};

void data_manager::mem_info() const {
	
	if (managers) {
		std::vector<manager *>::iterator it;
		for (it = managers->begin(); it != managers->end(); it++) 
			(*it)->mem_info();
	}
	
};

char * data_manager::types_info() const {
	
	char * res = NULL;
	
	if (managers) {
		std::vector<manager *>::iterator it;
		for (it = managers->begin(); it != managers->end(); it++) {

			char * types = (*it)->types_info();
			if (!types)
				continue;

			int res_len = 0;
			if (res)
				res_len = strlen(res);

			res = (char*) realloc( res, (res_len+strlen(types)+1)*sizeof(char) );
			strcpy(res+res_len, types);
	
		}
	}

	return res;

};

void data_manager::getMinMaxZ(REAL & minZ, REAL & maxZ) const {
	
	minZ = FLT_MAX;
	maxZ = -FLT_MAX;

	int count = data_count();
	int i;
	for (i = 0; i < count; i++) {
		REAL minz = FLT_MAX;
		REAL maxz = -FLT_MAX;
		const data * dat = data_get(i);
		if (dat->getMinMaxZ(minz, maxz)) {
			minZ = MIN(minZ, minz);
			maxZ = MAX(maxZ, maxz);
		};
	}
		
};

bool data_manager::bounds(REAL & Minx, REAL & Maxx, REAL & Miny, REAL & Maxy) const {
	
	Minx = FLT_MAX;
	Maxx = -FLT_MAX;
	Miny = FLT_MAX;
	Maxy = -FLT_MAX;

	bool Res = false;

	int count = data_count();
	int i;
	for (i = 0; i < count; i++) {
		REAL minx = FLT_MAX;
		REAL maxx = -FLT_MAX;
		REAL miny = FLT_MAX;
		REAL maxy = -FLT_MAX;
		const data * dat = data_get(i);
		bool res = dat->bounds(minx, maxx, miny, maxy);
		if (res) {
			Res = true;
			Minx = MIN(minx, Minx);
			Maxx = MAX(maxx, Maxx);
			Miny = MIN(miny, Miny);
			Maxy = MAX(maxy, Maxy);
		};
	}

	return Res;
		
};

bool data_manager::save(datafile * df) const {
	if (managers) {
		std::vector<manager *>::iterator it;
		for (it = managers->begin(); it != managers->end(); it++)
			if ((*it)->save(df) == false)
				return false;
	}
	return true;
};

int data_manager::data_count() const {
	int res = 0;
	if (managers) {
		std::vector<manager *>::iterator it;
		for (it = managers->begin(); it != managers->end(); it++)
			res += (*it)->data_count();
	}
	return res;
};

const data * data_manager::data_get(int i) const {
	int sum = 0;
	int res = 0;
	if (managers) {
		std::vector<manager *>::iterator it;
		for (it = managers->begin(); it != managers->end(); it++) {
			res = (*it)->data_count();
			if (sum + res > i) {
				return (*it)->data_get(i-sum);
			}
			sum += res;
		};
	};
	return NULL;
};

const data * data_manager::data_get(const char * type, const char * name) const {
	int i;
	for (i = 0; i < data_count(); i++) {
		const data * dat = data_get(i);
		if (dat->getType() && dat->getName())
			if ( (strcmp(dat->getType(), type) == 0) && (strcmp(dat->getName(), name) == 0))
				return dat;
	};
	return NULL;
};

//////////////////////////////
//
// surfit_manager
//
//////////////////////////////

void surfit_manager::release() {
	delete this;
};

int surfit_manager::load_tag(datafile * df, char * tagname) const {
	
	if (strcmp(tagname, "points") == 0) {
		
		d_points * res = _pnts_load_df(df, NULL);
		
		if (res) {
			surfit_pnts->push_back(res);
			_pnts_info(res);
		}
		
		if ( !df->readTagName(tagname) )
			return -1;
		
		return 1;
	}
	
	if (strcmp(tagname, "func") == 0) {
		
		d_func * res = _func_load_df(df, NULL);
		
		if (res) {
			surfit_funcs->push_back(res);
			_func_info(res);
		}
		
		if ( !df->readTagName(tagname) )
			return -1;
		
		return 1;
	}
	
	if (strcmp(tagname, "mask") == 0) {
		
		d_mask * res = _mask_load_df(df, NULL);
		
		if (res) {
			surfit_masks->push_back(res);
			_mask_info(res);
		}
		
		if ( !df->readTagName(tagname) )
			return -1;
		
		return 1;
	}
	
	if (strcmp(tagname, "cntr") == 0) {
		
		d_cntr * res = _cntr_load_df(df, NULL);
		if (res) {
			_cntr_info(res);
			surfit_cntrs->push_back(res);
		}
		
		if ( !df->readTagName(tagname) ) 
			return -1;
		
		return 1;
	}
	
	if (strcmp(tagname, "curv") == 0) {
		
		d_curv * res = _curv_load_df(df, NULL);
		if (res) {
			_curv_info(res);
			surfit_curvs->push_back(res);
		}
		
		if ( !df->readTagName(tagname) ) 
			return -1;
		
		return 1;
	}

	if (strcmp(tagname, "area") == 0) {
		
		d_area * res = _area_load_df(df, NULL);
		if (res) {
			_area_info(res);
			surfit_areas->push_back(res);
		}
		
		if ( !df->readTagName(tagname) ) 
			return -1;
		
		return 1;
	}
	
	if (strcmp(tagname, "grid") == 0) {
		
		d_grid * res = _grid_load_df(df, NULL);
		if (res) {
			delete surfit_grid;
			surfit_grid = res;
			grid_info();
		}
		
		if ( !df->readTagName(tagname) ) 
			return -1;
		
		return 1;
	}
	
	return 0;
};

bool surfit_manager::save(datafile *df) const {

	unsigned int cnt;

	if (surfit_grid) {
		if (!_grid_save_df(surfit_grid, df))
			return false;
	}

	for (cnt = 0; cnt < surfit_funcs->size(); cnt++) {
		d_func * fnc = *(surfit_funcs->begin()+cnt);
		if (!_func_save_df(fnc, df))
			return false;
	}

	for (cnt = 0; cnt < surfit_masks->size(); cnt++) {
		d_mask * fnc = *(surfit_masks->begin()+cnt);
		if (!_mask_save_df(fnc, df))
			return false;
	}

	for (cnt = 0; cnt < surfit_pnts->size(); cnt++) {
		d_points * pnts = *(surfit_pnts->begin()+cnt);
		if (!_pnts_save_df(pnts, df))
			return false;
	}

	for (cnt = 0; cnt < surfit_cntrs->size(); cnt++) {
		d_cntr * cntrline = *(surfit_cntrs->begin()+cnt);
		if (!_cntr_save_df(cntrline, df))
			return false;
	}

	for (cnt = 0; cnt < surfit_curvs->size(); cnt++) {
		d_curv * ar = *(surfit_curvs->begin()+cnt);
		if (!_curv_save_df(ar, df))
			return false;
	}

	for (cnt = 0; cnt < surfit_areas->size(); cnt++) {
		d_area * ar = *(surfit_areas->begin()+cnt);
		if (!_area_save_df(ar, df))
			return false;
	}

	return true;

};

void surfit_manager::clear_data() const {

	grid_unload();
	
	func_delall();
	mask_delall();

	pnts_delall();

	curv_delall();
	area_delall();
	cntr_delall();
	
	grid_line_unload();

	tol = float(1e-5);
	penalty_max_iter = 150;
	penalty_weight = 100;
	penalty_weight_mult = 10;


};

#define SURFIT_TYPES "surfit func mask pnts curv area cntr grid\n"

char * surfit_manager::types_info() const {
	return SURFIT_TYPES;
};

void surfit_manager::mem_info() const {

	
	funcs_info();
	masks_info();
	pnts_info();
	curvs_info();
	areas_info();
	cntrs_info();
	grid_info();

};

int surfit_manager::data_count() const {
	int res = 0;

	res += surfit_funcs->size();
	res += surfit_masks->size(); 
	res += surfit_pnts->size(); 
	res += surfit_cntrs->size(); 
	res += surfit_curvs->size();
	res += surfit_areas->size();

	return res;
};

bool surfit_manager::auto_load(const char * filename, const char * first1024, int readed) const {

	if (first1024 == NULL)
		return false;
	
	if ( strncmp(first1024,"DSAA",4) == 0 ) {
		bool res = false;
		try {
			res = func_load_grd(filename);
		} catch (...) {
			return false;
		}
		return res;
	}

	int columns = calc_columns(first1024, readed);
	if (columns == 2) {
		bool res = false;
		try {
			res = curv_read(filename);
		} catch (...) {
			return false;
		}
		return res;
	}
	
	if (columns == 3) {
		bool res = false;
		try {
			res = pnts_read(filename);
		} catch (...) {
			return false;
		}
		return res;
	}

	return false;
};

const data * surfit_manager::data_get(int i) const {
	
	int res = 0;
	int sum = 0;

	res = surfit_funcs->size();
	if (res + sum > i)
		return (*surfit_funcs)[i-sum];
	sum += res;

	res = surfit_masks->size();
	if (res + sum > i)
		return (*surfit_masks)[i-sum];
	sum += res;

	res = surfit_pnts->size();
	if (res + sum > i)
		return (*surfit_pnts)[i-sum];
	sum += res;

	res = surfit_cntrs->size();
	if (res + sum > i)
		return (*surfit_cntrs)[i-sum];
	sum += res;

	res = surfit_curvs->size();
	if (res + sum > i)
		return (*surfit_curvs)[i-sum];
	sum += res;

	res = surfit_areas->size();
	if (res + sum > i)
		return (*surfit_areas)[i-sum];
	sum += res;

	return NULL;
};

void manager::getMinMaxZ(REAL & minz, REAL & maxz) const {
	
	REAL minZ, maxZ;

	int max_data = data_count();
	int i;
	for (i = 0; i < max_data; i++) {
		const data  * dat = data_get(i);
		dat->getMinMaxZ(minZ, maxZ);
		minz = MIN(minz, minZ);
		maxz = MAX(maxz, maxZ);
	};

};

bool manager::bounds(REAL & minx, REAL & maxx, REAL & miny, REAL & maxy) const {
	
	REAL minX, maxX;
	REAL minY, maxY;

	bool reS, Res = false;

	int max_data = data_count();
	int i;
	for (i = 0; i < max_data; i++) {
		const data  * dat = data_get(i);
		reS = dat->bounds(minX, maxX, minY, maxY);
		if (reS) {
			Res = true;
			minx = MIN(minx, minX);
			maxx = MAX(maxx, maxX);
			miny = MIN(miny, minY);
			maxy = MAX(maxy, maxY);
		}

	};

	return Res;
};

///////////////////////
//
// Tcl functions
//
//////////////////////

void clear_data() {

	clear_cmofs();

	if (surfit_data_manager)
		surfit_data_manager->clear_data();

};

void mem_info() {

	if (surfit_data_manager)
		surfit_data_manager->mem_info();

	if (functionals->size() > 0) {
		int q;
		for (q = 0; q < (int)functionals->size(); q++) {
			functional * f = (*functionals)[q];
			f->info();
		}
	};

};

char * types_info() {

	if (surfit_data_manager)
		return surfit_data_manager->types_info();
	return NULL;
};

void file_load(const char * filename) {

	int readed;
	char * first1024 = read_first1024(filename, readed);

	if (first1024 == NULL)
		return;

	if ( strncmp(first1024,"roff",4) ==  0) {

		datafile *df = new datafile(filename, DF_MODE_READ); // read
		
		if (!df->condition()) {
			delete df;
			free(first1024);
			return;
		}
		
		if (surfit_data_manager)
			surfit_data_manager->load(df);
		
		
		sstuff_free_char(first1024);
		delete df;
		return;
	}
	
	if (surfit_data_manager)
		surfit_data_manager->auto_load(filename, first1024, readed);
	
	sstuff_free_char(first1024);
	
};

bool file_save(const char * filename) {

	bool res = true;
	bool op = true;
	
	datafile *df = new datafile(filename, DF_MODE_WRITE); // write
	if (!df->condition())
		return false;

	
	if (surfit_data_manager)
		res = surfit_data_manager->save(df);
	

	op = df->writeEof(); res = (op && res);

	delete df;
	return res;

};

}; // namespace surfit;

