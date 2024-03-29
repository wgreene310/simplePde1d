// Copyright (C) 2016 William H. Greene
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses/>.

#ifndef PDE1dMexInt_h
#define PDE1dMexInt_h

#include <algorithm>

#include <mex.h>

#include <PDE1dDefn.h>

class PDE1dMexInt : public PDE1dDefn
{
public:
  PDE1dMexInt(int m, const mxArray *pdefun, const mxArray *icfun,
    const mxArray *bcfun,
    const mxArray *xmesh, const mxArray *tspan);
  ~PDE1dMexInt();
  virtual int getNumEquations();
  virtual int getCoordSystem() {
    return mCoord;
  }
  int numNodes() { return mesh.size();  }
  virtual void evalIC(double x, RealVector &ic);
  virtual void evalBC(double xl, const RealVector &ul,
    double xr, const RealVector &ur, double t, BC &bc);
  virtual void evalPDE(double x, double t,
    const RealVector &u, const RealVector &DuDx, PDE &pde);
  virtual bool hasVectorPDEEval() const { return true; }
  virtual void evalPDE(RealVector x, double t,
    const RealMatrix &u, const RealMatrix &DuDx, PDEVec &pde);
  virtual RealVector getMesh();
  virtual RealVector getTimeSpan();
private:
  static void setScalar(double x, mxArray *a) {
    double *p = mxGetPr(a); p[0] = x;
  }
  template<class T>
  static void setMxImpl(const T &ea, mxArray *a) {
    int vr = ea.rows(), vc = ea.cols(), s = ea.size();
    if (vr != mxGetM(a) || vc != mxGetN(a)) {
      double *pr = (double*)mxRealloc(mxGetPr(a), s*sizeof(double));
      mxSetM(a, vr);
      mxSetN(a, vc);
      mxSetPr(a, pr);
    }
    std::copy_n(ea.data(), s, mxGetPr(a));
  }
  static void setVector(const RealVector &v, mxArray *a) {
    setMxImpl(v, a);
  }
  static void setMatrix(const RealMatrix &v, mxArray *a) {
    setMxImpl(v, a);
  }
  void setNumPde();
  void callMatlab(const mxArray *inArgs[], int nargin,
    RealVector *outArgs[], int nargout);
  void callMatlab(const mxArray *inArgs[], int nargin,
    RealMatrix *outArgs[], int nargout);
  static std::string getFuncNameFromHandle(const mxArray *fh);
  int mCoord, numPDE;
  RealVector mesh, tSpan;
  const mxArray *pdefun, *icfun, *bcfun, *xmesh, *tspan;

  mxArray *mxX1, *mxX2, *mxT; // scalar x and t
  mxArray *mxVec1, *mxVec2;
  static const int maxMatlabRetArgs = 4;
  mxArray *matOutArgs[maxMatlabRetArgs];
};

#endif

