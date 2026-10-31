#pragma GCC optimize("trapv")
#include <bits/stdc++.h>

using namespace std;
template<class T, class U = T> struct fraction {

	T a;
	T b;

	fraction() : a(0), b(1) {}
	fraction(T x) : a(x), b(1) {}
	fraction(T _a, T _b) : a(_a), b(_b) {
		assert(b != 0);
		if (b < 0) {
			b = -b;
			a = -a;
		}
		// reduce()
	}
	friend std::ostream& operator << (std::ostream& out, const fraction& n) { return out << n.a << '/' << n.b; }

	fraction neg() const {
		fraction res {-a, b};
		return res;
	}
	friend fraction neg(const fraction& m) { return m.neg(); }

	fraction operator- () const {
		return neg();
	}
	fraction operator+ () const {
		return fraction(*this);
	}

	friend bool operator == (const fraction& x, const fraction& y){
		return U(x.a) * y.b == U(x.b) * y.a;
	}
	friend bool operator != (const fraction& x, const fraction& y){
		return U(x.a) * y.b != U(x.b) * y.a;
	}
	friend bool operator < (const fraction& x, const fraction& y){
		return U(x.a) * y.b < U(x.b) * y.a;
	}
	friend bool operator <= (const fraction& x, const fraction& y){
		return U(x.a) * y.b <= U(x.b) * y.a;
	}
	friend bool operator > (const fraction& x, const fraction& y){
		return U(x.a) * y.b > U(x.b) * y.a;
	}
	friend bool operator >= (const fraction& x, const fraction& y){
		return U(x.a) * y.b >= U(x.b) * y.a;
	}

	void reduce(){
		T g = std::gcd(std::abs(a), std::abs(b));
		a /= g;
		b /= g;
		if(b < 0) { a = -a; b = -b; }
	}

	fraction& operator += (const fraction& o){
		*this = {a * o.b + b * o.a, b * o.b};
		//reduce();
		return *this;
	}
	fraction& operator -= (const fraction& o){
		*this = {a * o.b - b * o.a, b * o.b};
		//reduce();
		return *this;
	}
	fraction& operator *= (const fraction& o){
		*this = {a * o.a, b * o.b};
		//reduce();
		return *this;
	}
	fraction& operator /= (const fraction& o){
		*this = {a * o.b, b * o.a};
		//reduce();
		return *this;
	}
	friend fraction operator + (const fraction& a, const fraction& b) { return fraction(a) += b; }
	friend fraction operator - (const fraction& a, const fraction& b) { return fraction(a) -= b; }
	friend fraction operator * (const fraction& a, const fraction& b) { return fraction(a) *= b; }
	friend fraction operator / (const fraction& a, const fraction& b) { return fraction(a) /= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t XA, YA, DA; cin >> XA >> YA >> DA;
		int64_t XB, YB, DB; cin >> XB >> YB >> DB;
		int64_t XC, YC, DC; cin >> XC >> YC >> DC;

		using ld = long double;
		auto sq = [](ld a) -> ld { return a * a; };
		auto solve = [&]() -> std::pair<int, ld> {
			using frac_t = fraction<__int128_t, __int128_t>;
			assert(YA == 0 && YB == 0);

			int64_t min_R = std::max({DA, DB, DC});

			// e[0] * x + e[1] * y = e[2] * r + e[3]
			std::array<int64_t, 4> eA{-2 * XA, -2 * YA, -2 * DA, - XA * XA - YA * YA + DA * DA};
			std::array<int64_t, 4> eB{-2 * XB, -2 * YB, -2 * DB, - XB * XB - YB * YB + DB * DB};
			std::array<int64_t, 4> eC{-2 * XC, -2 * YC, -2 * DC, - XC * XC - YC * YC + DC * DC};

			std::array<int64_t, 4> eP = {eA[0] - eB[0], eA[1] - eB[1], eA[2] - eB[2], eA[3] - eB[3]};
			std::array<int64_t, 4> eQ = {eA[0] - eC[0], eA[1] - eC[1], eA[2] - eC[2], eA[3] - eC[3]};
			assert(eP[0] != 0);
			assert(eP[1] == 0);
			if (YC == 0) {
				assert(eQ[1] == 0);
				assert(eQ[0] != 0);
				int64_t a = eP[0];
				int64_t b = eQ[0];
				for (int i = 0; i < 4; i++) eQ[i] = a * eQ[i] - eP[i] * b;
				assert(eQ[0] == 0);
				assert(eQ[1] == 0);

				if (eQ[2] == 0) {
					if (eQ[3] != 0) {
						return {0, 0};
					}
					// otherwise, we have just eP and the quadratic, with one free variable??
					// I think in this case, AMGM breaks or something?
					// They're linear, so the solution should be line
					// TODO: Checkme
					if (eP[2] == eP[0]) {
						return {-1, 0};
					} else {
						return {0, 0};
					}
				} else {
					// we have x
					frac_t r{-eQ[3], eQ[2]};
					frac_t x = (eP[2] * r + frac_t(eP[3])) * frac_t{1, eP[0]};
					if (r < min_R) {
						return {0, 0};
					}
					frac_t y2 = (r - DA) * (r - DA) - (x - XA) * (x - XA);
					if (y2 < 0) {
						return {0, 0};
					} else if (y2 == 0) {
						return {1, ld(r.a) / ld(r.b)};
					} else {
						return {2, ld(r.a) / ld(r.b)};
					}
				}
			} else {
				assert(eQ[1] != 0);
				if (eQ[0] != 0) {
					int64_t a = eP[0];
					int64_t b = eQ[0];
					for (int i = 0; i < 4; i++) eQ[i] = a * eQ[i] - eP[i] * b;
					assert(eQ[0] == 0);
					assert(eQ[1] != 0);
				}
				assert(eQ[1] != 0);
				// now we have x and y as linear combinations of r and c
				// now we plug them back in
				assert(eP[1] == 0);
				frac_t Xr{eP[2], eP[0]};
				frac_t Xv{eP[3], eP[0]};

				assert(eQ[0] == 0);
				frac_t Yr{eQ[2], eQ[1]};
				frac_t Yv{eQ[3], eQ[1]};

				// now, let's do the quadratic
				Xv -= XA;
				Yv -= YA;
				frac_t v2 = Xr * Xr + Yr * Yr - 1;
				frac_t v1 = Xr * Xv + Yr * Yv + DA; // b / 2
				frac_t v0 = Xv * Xv + Yv * Yv - DA * DA;
				assert(v0.b == v1.b && v1.b == v2.b);
				v0.b = v1.b = v2.b = 1;
				if (v2 < 0) {
					v2 = -v2, v1 = -v1, v0 = -v0;
				}
				if (v2 == 0) {
					if (v1 == 0) {
						if (v0 != 0) {
							return {0, 0};
						} else {
							// what, all radii? is this real?
							return {-1, 0};
						}
					}
					v1 *= 2;
					frac_t r = v0 / (-v1);
					if (r < min_R) {
						return {0, 0};
					} else {
						return {1, ld(r.a) / ld(r.b)};
					}
				} else {
					//cerr << "quadratic case" << '\n';
					frac_t cons = -v1;
					frac_t rt = v1 * v1 - v2 * v0;
					// divide by v2 later

					//cerr << int64_t(cons.a) << ' ' << int64_t(rt.a) << '\n';
					//cerr << int64_t(v2.a) * min_R << '\n';
					// -(b / 2) +- sqrt((b/2)^2 - c)
					if (rt < 0) {
						return {0, 0};
					} else if (rt == 0) {
						if (cons >= v2 * min_R) {
							return {1, ld(cons.a) / ld(v2.a)};
						} else {
							return {0, 0};
						}
					} else {
						//cerr << "checking signs" << '\n';
						ld r_big = (ld(cons.a) + std::sqrt(ld(rt.a))) / ld(v2.a);
						ld X_big = ld(r_big) * ld(Xr.a) / ld(Xr.b) + ld(Xv.a) / ld(Xv.b) + XA;
						ld Y_big = ld(r_big) * ld(Yr.a) / ld(Yr.b) + ld(Yv.a) / ld(Yv.b) + YA;
						//cerr << "candidate circle " << r_big << " at " << X_big << ',' << Y_big << '\n';
						//cerr << sq(X_big - XA) + sq(Y_big - YA) << ' ' << sq(r_big - DA) << '\n';
						//cerr << sq(X_big - XB) + sq(Y_big - YB) << ' ' << sq(r_big - DB) << '\n';
						//cerr << sq(X_big - XC) + sq(Y_big - YC) << ' ' << sq(r_big - DC) << '\n';

						ld r_small = (ld(cons.a) - std::sqrt(ld(rt.a))) / ld(v2.a);
						ld X_small = ld(r_small) * ld(Xr.a) / ld(Xr.b) + ld(Xv.a) / ld(Xv.b) + XA;
						ld Y_small = ld(r_small) * ld(Yr.a) / ld(Yr.b) + ld(Yv.a) / ld(Yv.b) + YA;
						//cerr << "candidate circle " << r_small << " at " << X_small << ',' << Y_small << '\n';
						//cerr << sq(X_small - XA) + sq(Y_small - YA) << ' ' << sq(r_small - DA) << '\n';
						//cerr << sq(X_small - XB) + sq(Y_small - YB) << ' ' << sq(r_small - DB) << '\n';
						//cerr << sq(X_small - XC) + sq(Y_small - YC) << ' ' << sq(r_small - DC) << '\n';
						//cerr << r_small << ' ' << r_big << '\n';
						// rt > 0
						frac_t rhs = v2 * min_R - cons;
						if (rhs <= 0 || rt >= rhs * rhs) {
							if (rhs <= 0 && rt <= rhs * rhs) {
								return {2, r_small};
							}
							return {1, r_big};
						}
						return {0, 0};
					}
				}

			}
		};

		auto [ways, v] = solve();
		if (ways == -1) {
			cout << -1 << '\n';
		} else if (ways == 0) {
			cout << 0 << '\n';
		} else {
			assert(ways == 1 || ways == 2);
			cout << ways << ' ' << fixed << setprecision(12) << v << '\n';
		}
	}

	return 0;
}
