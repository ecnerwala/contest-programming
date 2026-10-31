#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std ;
double TAU = 6.283185307179586 ;
double PI = TAU * 0.5 ;
int verbose ;
pair<double, double> timefor(double x, double y, double v, double w) {
   double r = v / w ;
   y = abs(y) ;
   double d = hypot(x, y) ;
   double theta = atan2(y, x) ;
/*
 *   We arc towards until we are pointing exactly at the
 *   target.  We need to sum our rotational velocity and our angular
 *   movement around the target itself.  We compute this by calculating
 *   the intersection of a circle and a tangent line.
 */
   double rdir = PI * 0.5 ;
   if (verbose) cout << "Rdir is " << rdir << endl ;
   double rx = 0 ;
   double ry = r ;
   if (verbose) cout << "Center of circle is " << rx << " " << ry << endl ;
   double hd = hypot(rx-x, ry-y) ;
   if (verbose) cout << "hd " << hd << " r " << r << endl ;
   if (verbose) cout << "atan " << atan2(y-ry, x-rx) << " acos " << acos(r/hd) << endl ;
   double ft1 = atan2(y-ry, x-rx) - acos(r/hd) + PI * 0.5 ;
   double ft2 = -atan2(y-ry, x-rx) - acos(r/hd) - PI * 0.5 ;
   while (ft1 < 0)
      ft1 += TAU ;
   while (ft2 < 0)
      ft2 += TAU ;
   if (verbose) cout << "Ft1 " << ft1 << " ft2 " << ft2 << endl ;
   double finaltheta = min(ft1, ft1) ;
   if (verbose) cout << "Final theta " << finaltheta << endl ;
   double arctime = finaltheta / w ;
   double linetime = 0 ;
   double lastseg = hd*hd-r*r ;
   if (lastseg > 0)
      linetime = sqrt(lastseg) / v ;
   if (verbose) cout << "Times: "  << arctime << " " << linetime << endl ;
/*
 *   Okay, we have our three times; now let's simulate it and see where
 *   we end up.
 */
   return make_pair(arctime, linetime) ;
}
int main(int argc, char *argv[]) {
   verbose = argc > 1 ;
   double x, y, v, w ;
   cout << setprecision(15) ;
   cin >> x >> y >> v >> w ;
   y = abs(y) ; // put it above the x plane; keeps signs simple
   double r = v / w ;
   double maxang = abs(atan2(y, x)) ; // guard against -PI return
   // try different angles of spin
   double lo = 0 ;
   double hi = maxang ;
   double spintime=0, arctime=0, linetime=0 ;
   for (int iter=0; iter<80; iter++) {
      double best = -1 ;
      double loval = 1e99 ;
      for (int i=0; i<=100; i++) {
         double ang = lo + (hi - lo) * i / 100 ;
         double spin = ang / w ;
         pair<double, double> r =
                 timefor(x*cos(ang)+y*sin(ang), y*cos(ang)-x*sin(ang), v, w) ;
         double rr = r.first + r.second + spin ;
         if (rr < loval) {
            loval = rr ;
            best = i ;
            spintime = spin ;
            arctime = r.first ;
            linetime = r.second ;
         }
      }
      double nlo = lo + (hi - lo) * max(0.0, best-10) / 100 ;
      double nhi = lo + (hi - lo) * min(100.0, best+10) / 100 ;
      hi = nhi ;
      lo = nlo ;
   }
   if (verbose) cout << "Final result " << spintime << " " << arctime << " " << linetime << endl ;
/*
 *   Okay, we have our three times; now let's simulate it and see where
 *   we end up.
 */
   double theta = spintime * w ;
   double efftheta = theta + 0.5 * w * arctime ;
   double dist = 2 * r * sin(0.5 * w * arctime) ;
   double xx = dist * cos(efftheta) ;
   double yy = dist * sin(efftheta) ;
   if (verbose) cout << "Tangency is at " << xx << " " << yy << endl ;
   theta += arctime * w ;
   xx += v * linetime * cos(theta) ;
   yy += v * linetime * sin(theta) ;
   double dd = hypot(x-xx, y-yy) ;
   if (dd > 1e-6 || linetime < 0 || arctime < 0 || spintime < 0) {
      verbose = 1 ;
      cout << "FAILED! d is " << dd << endl ;
   }
   if (verbose) cout << "Goal was " << x << " " << y << " got to " << xx << " " << yy << endl ;
   double tottime = linetime + arctime + spintime ;
   cout << tottime << endl ;
}
