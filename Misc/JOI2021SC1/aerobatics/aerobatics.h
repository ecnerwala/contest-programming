#include <cmath>
#include <algorithm>

double GetAngle(int xa, int ya, int xb, int yb, int xc, int yc) {
  xb -= xa;
  yb -= ya;
  xc -= xa;
  yc -= ya;
  double dot = 1.0 * xb * xc + 1.0 * yb * yc;
  double normb = 1.0 * xb * xb + 1.0 * yb * yb;
  double normc = 1.0 * xc * xc + 1.0 * yc * yc;
  return std::acos(
             std::min(std::max(dot / std::sqrt(normb * normc), -1.0), 1.0)) *
         180.0 / 3.14159265358979323846;
}
