// Problem         : Rocket Ship (NAIPC 2019)
// Author          : Darcy Best
// Expected Result : AC
// Complexity      : O(log(PRECISION))
 
// Once you know how long both rockets are activated, the other two are
// determinable

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <cstdlib>
using namespace std;

const double PI = acos(-1.0);
const double EPS = 1e-12;

double naive_answer; // Rotate first, then move
double x,y,w,v;      // Input
double r,R;          // Radii of the circles
double phi;

double sgn(double x){ return x < 0 ? -1 : 1; }

double g(double theta){
  double X = r * cos(theta - PI/2);
  double Y = r * sin(theta - PI/2) + r;

  if(hypot(X,Y) > R) return naive_answer; // bad!

  double a = abs(Y - r), b = abs(X);

  if(atan2(Y-r,X) > 0) a = -a;
  if(X < 0) b = -b;
  
  double tA = a*a*(R*R-Y*Y) + 2*a*b*X*Y + b*b*(R*R-X*X);
  double tB = a * X + b * Y;
  double tC = a*a + b*b;

  double t = max((-sqrt(tA) - tB) / tC,(sqrt(tA) - tB) / tC);

  double Xp = X + a*t, Yp = Y + b*t;

  double phi_p = atan2(Yp,Xp);
  if(phi_p < 0) phi_p += 2*PI;

  if(phi_p - phi > EPS) return naive_answer;

  return (phi - phi_p) / w + (hypot(Xp-X,Yp-Y) + r * theta) / v;
}

double f(double lo, double hi){
  double q1 = (2*lo + hi)/3, q2 = (lo + 2*hi)/3;
  double a = g(q1), b = g(q2);

  if(fabs(lo-hi) < EPS && fabs(a-b) < EPS) return (a+b)/2;
  return a > b ? f(q1,hi) : f(lo,q2);
}

int main(){
  cin >> x >> y >> v >> w;

  y = abs(y);
  r = v / w;
  R = hypot(x,y);
  phi = atan2(y,x);

  naive_answer = g(0);

  cout << fixed << setprecision(10) << min(f(0,PI/2),f(PI/2,PI)) << endl;
}
