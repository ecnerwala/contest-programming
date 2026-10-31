#include <bits/stdc++.h>

using dbl = double;

namespace kactl {
using namespace std;
using d = dbl;
#define S(a,b) (f(a) + 4*f((a+b) / 2) + f(b)) * (b-a) / 6

template <class F>
d rec(F& f, d a, d b, d eps, d S) {
	d c = (a + b) / 2;
	d S1 = S(a, c), S2 = S(c, b), T = S1 + S2;
	if (abs(T - S) <= 15 * eps || b - a < 1e-10)
		return T + (T - S) / 15;
	return rec(f, a, c, eps / 2, S1) + rec(f, c, b, eps / 2, S2);
}
template<class F>
d quad(d a, d b, F f, d eps) {
	return rec(f, a, b, eps, S(a, b));
}

}

std::pair<dbl, dbl> solve(int D, int R, int T_final) {
	const dbl PI = std::acos(dbl(-1));
	dbl theta_hi;
	if (D == R) {
		theta_hi = PI / 2;
	} else if (R > D) {
		theta_hi = PI;
	} else if (R < D) {
		theta_hi = std::asin(dbl(R)/dbl(D));
	} else assert(false);
	auto solve_at_time = [&](dbl t) -> dbl {
		// variance of each variable
		auto at_ang = [&](dbl theta) -> dbl {
			if (theta > theta_hi) return 0;
			dbl h = dbl(D) * std::sin(theta);
			dbl d_mid = dbl(D) * std::cos(theta);
			dbl d_side = std::sqrt(std::max(dbl(0), dbl(R) * dbl(R) - h * h));
			dbl d_lo = (R < D) ? d_mid - d_side : 0;
			dbl d_hi = d_mid + d_side;
			if (t <= 1e-10) {
				return (R < D) ? 0 : 1;
			}
			dbl val = (R < D) ? std::exp(-d_lo * d_lo / t / 1e4) : 1;
			val -= std::exp(-d_hi * d_hi / t / 1e4);
			//std::cerr << theta << ' ' << h << ' ' << d_lo << ' ' << d_hi << ' ' << val << '\n';
			return val;
		};
		return kactl::quad(0, theta_hi, at_ang, 0.2e-8) / PI;
	};
	dbl a1 = solve_at_time(T_final);
	dbl a2 = kactl::quad(0, T_final, solve_at_time, 0.2e-8 * T_final / 1e-4);
	return {a1, a2};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int K; cin >> K;
	while (K--) {
		auto read_double = [&]() -> int {
			double v; cin >> v;
			v *= 1e4;
			int v_int = int(v+0.5);
			assert(std::abs(v_int - v) <= 1e-6);
			return v_int;
		};
		int D = read_double();
		int R = read_double();
		int T = read_double();
		auto [a, b] = solve(D, R, T);
		b /= 1e4;
		cout << fixed << setprecision(20);
		cout << a << ' ' << b << '\n';
	}

	return 0;
}
