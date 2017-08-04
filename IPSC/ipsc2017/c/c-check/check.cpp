#include "testlib.h"
#include <cmath>

using namespace std;

const double EPS = 1E-6;

inline bool doubleCompareX(double expected, double result, double MAX_DOUBLE_ERROR) {
    if (expected == 0.0) return result == 0.0; 
	if (__testlib_isNaN(expected)) return __testlib_isNaN(result);
    if (__testlib_isInfinite(expected)) {
    	if (expected > 0) {
			return result > 0 && __testlib_isInfinite(result);
        } else {
            return result < 0 && __testlib_isInfinite(result);
        }
    }
    if (__testlib_isNaN(result) || __testlib_isInfinite(result) || result == 0) return false;
    double minv = __testlib_min(expected * (1.0 - MAX_DOUBLE_ERROR), expected * (1.0 + MAX_DOUBLE_ERROR));
    double maxv = __testlib_max(expected * (1.0 - MAX_DOUBLE_ERROR), expected * (1.0 + MAX_DOUBLE_ERROR));
    return result >= minv && result <= maxv;
}

inline double doubleDeltaX(double expected, double result)
{
    double absolute = __testlib_abs(result - expected);        
    return expected == 0 ? absolute : __testlib_abs(absolute / expected);
}

int main(int argc, char * argv[])
{
    setName("compare N doubles, max relative error = %.10f", EPS);
    registerTestlibCmd(argc, argv);

    double maxE = 0.0;
    int T = inf.readInt();
    for (int t = 1; t <= T; ++t) {
        double j = ans.readDouble();
        double p = ouf.readDouble();
        if (p < 0) {
            quitf(_wa, "test %d: got negative probability '%.10e', expected '%.10e'", t, p, j);
        }

        if (p > 1) {
            quitf(_wa, "test %d: got too high probability '%.10e', expected '%.10e'", t, p, j);
        }

        if (!doubleCompareX(j, p, EPS)) {
            quitf(_wa, "%d%s numbers differ - expected: '%.10e', found: '%.10e', error = '%.10e'",
                t, englishEnding(t).c_str(), j, p, doubleDeltaX(j, p));
        }

        maxE = max(maxE, doubleDeltaX(j, p));
    }

    quitf(_ok, "%d numbers, max error = '%.10e'", T, maxE);
}
