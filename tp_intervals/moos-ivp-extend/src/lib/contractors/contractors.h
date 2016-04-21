/* ============================================================================
 *  IBEX-Robotics - Robotics contractors
 * ============================================================================
 *  Copyright : Luc Jaulin, Fabrice Le Bars, Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Luc Jaulin, Fabrice Le Bars, Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#ifndef Contractors_HEADER
#define Contractors_HEADER

#include "ibex/ibex.h"

void ctcInRing(ibex::Interval& x, ibex::Interval& y, double cx, double cy, const ibex::Interval& r);
void sepInRing(ibex::Interval& x, ibex::Interval& y, double cx, double cy, const ibex::Interval& r, bool outer);
void bisect(const ibex::IntervalVector& x, ibex::IntervalVector& x1, ibex::IntervalVector& x2);

#endif