
/*------------------------------------------------------------------------------
 *	$Id$
 *
 *	Copyright (c) 2002-2004 by M. V. Dmitrievsky and V. N. Kutrunov
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

#ifndef __surfit__grid__
#define __surfit__grid__

namespace surfit {

class datafile;
class task;

#include <math.h>

/*! \class grid
    \brief class for equidistant 2d-grid

This class describes equidistant rectangular grid (rectangular region comprised of evenly 
spaced rows and columns), presented with cells (intersection of a rows and columns). Each 
cell have center point, which
is grid node. To set the grid it is necessary to specify "start" and "end" points, distance between 
two neighbour nodes in OX and OY directions (stepX and stepY respectively). On the picture cell border
shown with dashed lines, nodes with small circles.
Gridding generates a Z value at each grid node by interpolating or approximating the \ref data values. 

Useful Tcl commands to set the grid are:
\ref grid_auto, \ref grid_get, \ref grid_get_for_task, \ref grid_get_for_func. 

To save and load grid use these: \ref grid_load, \ref grid_save
   
\image html grid.jpg
*/
class SURFIT_EXPORT grid {
public:

	/*! constructor
	    \param startX X-coordinate of the first node
	    \param endX X-coordinate of the last node
	    \param stepX X-distance between first and second nodes
	    \param startY Y-coordinate of the first node
	    \param endY Y-coordinate of the last nodes
	    \param stepY Y-distance between first and second nodes
	    \param iname grid name
	*/
	grid(REAL startX, REAL endX, REAL stepX,
	     REAL startY, REAL endY, REAL stepY,
	     const char * iname = NULL);

	//! copy constructor
	grid(const grid * igrid);

	//! destructor
        virtual ~grid();

	//! calculates the amount of nodes in X direction
	int getCountX() const;

	//! calculates the amount of nodes in Y direction
	int getCountY() const;

	//! returns coordinates of (i,j) node
	void getCoordNode(int i, int j, REAL & x, REAL & y) const;

	//! returns X-coordinate of (i,j) node
	REAL getCoordNodeX(int i) const { return startX + (i)*stepX; };

	//! returns Y-coordinate of (i,j) node
	REAL getCoordNodeY(int j) const { return startY + (j)*stepY; };
	
	//! calculates (i,j) of the cell for point with coords (x,y)
	void getCoordPoint(REAL x, REAL y, int & i, int & j) const;

	//! returns i number of (i,j) of the cell for point with coords (x,y)
	int get_i(REAL x) const {
		if (stepX == 0)	
			return -1;
		return (int)floor((x-startX)/stepX + 0.5);
	};

	//! returns j number of (i,j) of the cell for point with coords (x,y)
	int get_j(REAL y) const {
		if (stepY == 0)	
			return -1;
		return (int)floor((y-startY)/stepY + 0.5);
	};

	//! writes tag for saving grid to \ref datafile 
	bool writeTags(datafile *df) const;

	//! returns grid name 
	const char * getName() const;

	//! sets geometry name to newname 
	void setName(const char * newname);

	//! returns number of lines need to draw geometry
	int getLinesCount() const;

	//! returns coordinates of line with number line_number
	void getLine(int line_number, float &x0, float &y0, float &x1, float &y1) const;

	//! compares grid with another 
	bool operator==(const grid * test) const;

	//! X-coordinate of the first node
	REAL startX;
	//! X-coordinate of the last node
	REAL endX;
	//! X-distance between first and second nodes
	REAL stepX;
	//! Y-coordinate of the first node
	REAL startY;
	//! Y-coordinate of the last node
	REAL endY;
	//! Y-distance between first and second nodes
	REAL stepY;

protected:
	
	//! grid name 
	char * gridname;

};

}; // namespace surfit;

#endif

