
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

#ifndef __surfit_approx_points_tcl_included__
#define __surfit_approx_points_tcl_included__

namespace surfit {

/*! \ingroup tcl_rules_points
    \fn bool points(const char * points_name_or_position = "0");
    This rule approximates points in least squares meaning.
    If each cell contains no more than one point, result surface interpolates points.

    \par Tcl syntax:
    points "points_name_or_position"

    \par Math:
    This command adds the following functional to the functional sequence:
    \f[
    \Phi(u_{1,1},\ldots,u_{N,M}) = \sum_{p=1}^P \left( u_{i,j} - z_p \right)^2,
    \f]
    where points with coordinates \f$ (x_p, y_p) \f$ are belong to the cells (i,j)

*/
SURFIT_EXPORT
bool points(const char * points_name_or_position = "0");

/*! \ingroup tcl_rules_points
    \fn bool points_add(REAL weight = 1, const char * points_name_or_position = "0");
    This rule modifies previous (modifable) rule by adding \ref points rule with some weight.

    \par Tcl syntax:
    points_add weight "points_name_or_position"

    \par Math:
    This command modifies previous functional \f$ \Phi_0 \f$ by adding \f$ \Phi_1 \f$:
    \f[
    \Phi(u_{1,1},\ldots,u_{N,M}) = \Phi_0(u_{1,1},\ldots,u_{N,M}) + w\Phi_1(u_{1,1},\ldots,u_{N,M}),
    \f]
    where \f$ w \f$ - informational weight,
    \f[
    \Phi_1(u_{1,1},\ldots,u_{N,M}) = \sum_{p=1}^P \left( u_{i,j} - z_p \right)^2,
    \f]
    where points with coordinates \f$ (x_p, y_p) \f$ are belong to the cells (i,j)

*/
SURFIT_EXPORT
bool points_add(REAL weight = 1, const char * points_name_or_position = "0");

/*! \ingroup tcl_rules_points
    \fn bool points_leq(const char * points_name_or_position = "0", REAL mult = REAL(0.001));
    This rule adds surface condition - "the surface should be lower than equal to points".

    In case of algorithm bad convergence or unexpected (wrong) result, you should carefully review
    your conditions and if they are correct, try to change "mult" parameter

    \par Tcl syntax:
    points "points_name_or_position" mult

    \par Math:
    This command adds the condition:
    \f[
    u_{i,j} \leq z_p, \quad p=1,\ldots,P
    \f]
    where points with coordinates \f$ (x_p, y_p) \f$ are belong to the cells (i,j)

*/
SURFIT_EXPORT
bool points_leq(const char * points_name_or_position = "0", REAL mult = REAL(0.001));

/*! \ingroup tcl_rules_points
    \fn bool points_geq(const char * points_name_or_position = "0", REAL mult = REAL(0.001));
    This rule adds surface condition - "the surface should be greater than equal to points".

    In case of algorithm bad convergence or unexpected (wrong) result, you should carefully review
    your conditions and if they are correct, try to change "mult" parameter

    \par Tcl syntax:
    points_geq "points_name_or_position" mult

    \par Math:
    This command adds the condition:
    \f[
    u_{i,j} \geq z_p, \quad p=1,\ldots,P
    \f]
    where points with coordinates \f$ (x_p, y_p) \f$ are belong to the cells (i,j)

*/
SURFIT_EXPORT
bool points_geq(const char * points_name_or_position = "0", REAL mult = REAL(0.001));

}; // namespace surfit;

#endif
