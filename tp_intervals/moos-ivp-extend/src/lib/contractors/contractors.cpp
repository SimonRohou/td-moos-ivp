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

#include "contractors.h"
#include <iostream>

using namespace std;
using namespace ibex;

void ctcInRing(Interval& x, Interval& y, double cx, double cy, const Interval& r)
{
  static Variable var_x, var_y, var_r, var_vcx, var_vcy;
  static NumConstraint C(var_x, var_y, var_r, var_vcx, var_vcy,
                         pow(var_x-var_vcx,2) + pow(var_y-var_vcy,2) = pow(var_r,2));
  static CtcHC4 ctc(C);

  IntervalVector P(5); P[0]=x; P[1]=y; P[2]=r; P[3]=cx; P[4]=cy;
  ctc.contract(P);

  if(P.is_empty())
  {
    x = Interval::EMPTY_SET;
    y = x;
  }

  else
  {
    x = P[0];
    y = P[1];
  }
}

void sepInRing(Interval& x, Interval& y, double cx, double cy, const Interval& r, bool outer)
{
  if(outer)
  {
    ctcInRing(x, y, cx, cy, r);
    return;
  }

  Interval Xa(x), Ya(y), Xb(x), Yb(y);
  ctcInRing(Xa, Ya, cx, cy, Interval(-1, r.lb()));
  ctcInRing(Xb, Yb, cx, cy, Interval(r.ub(), POS_INFINITY));
  x = Xa | Xb;
  y = Ya | Yb;
}

void bisect(const IntervalVector& x, IntervalVector& x1, IntervalVector& x2)
{
  LargestFirst bisector(0., 0.45);
  pair<IntervalVector,IntervalVector> boxes = bisector.bisect(x);
  x1 = boxes.first;
  x2 = boxes.second;
}