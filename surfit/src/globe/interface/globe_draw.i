
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


%module globe
%{
#include "dem_show.h"
%}

namespace surfit {

extern int draw_dem;

%exception {
%#ifdef HAVE_GL_WORKS
        try {
                $action
        }
        catch(...) {
                return TCL_ERROR;
        }
%#else
        return TCL_ERROR;
%#endif
};

void dem_show (const char *filename=NULL, const char *demname=NULL);

};

