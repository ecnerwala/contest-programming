#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std ;
double TAU = 6.283185307179586 ;
double PI = TAU * 0.5 ;
int main(int argc, char *argv[]) {
   int verbose = argc > 1 ;
   double x, y, v, w ;
   cout << setprecision(15) ;
   cin >> x >> y >> v >> w ;
   y = abs(y) ; // put it above the x plane; keeps signs simple
   double r = v / w ;
   double d = hypot(x, y) ;
   double theta = atan2(y, x) ;
/*
 *   Always rotate, then rotate/go, then just go.
 *   Any components can be empty.
 *
 *   First part:  Do we rotate without translation?  We rotate if our
 *   angle is greater than an arc of radius r between us and goal, if
 *   one fits; if one doesn't fit, we rotate until we are at 90 degrees.
 */
   double goaltheta = theta - PI * 0.5 ;
   double mx = x * 0.5 ;
   double my = y * 0.5 ;
   double dp = r * r - mx * mx - my * my ;
   if (verbose) cout << "Goal theta is " << goaltheta << endl ;
   if (dp > 0)
      goaltheta = theta + atan2(sqrt(dp), d*0.5) - PI * 0.5 ;
   if (verbose) cout << "Goal theta is " << goaltheta << endl ;
   goaltheta = max(0.0, goaltheta) ;
   double spintime = 0 ;
   double curdir = 0 ;
   if (goaltheta > 0) {
      spintime = goaltheta / w ; // rotation time
      curdir = goaltheta ;
   }
/*
 *   Second part:  we arc towards until we are pointing exactly at the
 *   target.  We need to sum our rotational velocity and our angular
 *   movement around the target itself.  We compute this by calculating
 *   the intersection of a circle and a tangent line.
 */
   double rdir = curdir + PI * 0.5 ;
   if (verbose) cout << "Rdir is " << rdir << endl ;
   double rx = r * cos(rdir) ;
   double ry = r * sin(rdir) ;
   if (verbose) cout << "Center of circle is " << rx << " " << ry << endl ;
   double hd = hypot(rx-x, ry-y) ;
   if (verbose) cout << "hd " << hd << " r " << r << endl ;
   if (verbose) cout << "atan " << atan2(y-ry, x-rx) << " acos " << acos(r/hd) << endl ;
   double ft1 = atan2(y-ry, x-rx) - acos(r/hd) + PI * 0.5 ;
   double ft2 = atan2(y-ry, x-rx) + acos(r/hd) + PI * 0.5 ;
   if (ft1 < curdir)
      ft1 += TAU ;
   if (ft2 < curdir)
      ft2 += TAU ;
   if (verbose) cout << "Curdir " << curdir << " ft1 " << ft1 << " ft2 " << ft2 << endl ;
   double finaltheta = min(ft1, ft2) ;
   if (verbose) cout << "Final theta " << finaltheta << endl ;
   double arctime = (finaltheta - curdir) / w ;
   double linetime = 0 ;
   double lastseg = hd*hd-r*r ;
   if (lastseg > 0)
      linetime = sqrt(lastseg) / v ;
     if (verbose) cout << "Times: " << spintime << " " << arctime << " " << linetime << endl ;
/*
 *   Okay, we have our three times; now let's simulate it and see where
 *   we end up.
 */
   theta = spintime * w ;
   double efftheta = theta + 0.5 * w * arctime ;
   double dist = 2 * r * sin(0.5 * w * arctime) ;
   double xx = dist * cos(efftheta) ;
   double yy = dist * sin(efftheta) ;
   if (verbose) cout << "Tangency is at " << xx << " " << yy << endl ;
   theta += arctime * w ;
   xx += v * linetime * cos(theta) ;
   yy += v * linetime * sin(theta) ;
   double dd = hypot(x-xx, y-yy) ;
   if (dd > 1e-8 || linetime < 0 || arctime < 0 || spintime < 0) {
      verbose = 1 ;
      cout << "FAILED! " << endl ;
   }
   if (verbose) cout << "Goal was " << x << " " << y << " got to " << xx << " " << yy << endl ;
   double tottime = linetime + arctime + spintime ;
   cout << tottime << endl ;
}
