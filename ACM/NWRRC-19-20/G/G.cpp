#include<bits/stdc++.h>
using namespace std;

template <typename T> void setmax(T& a, const T& b) { if (a < b) a = b; }
template <typename T> void setmin(T& a, const T& b) { if (b < a) a = b; }

// number of integer solutions to Ax + By <= C and x,y >= 0
long long lattice_cnt(long long A, long long B, long long C) {
	using ll = long long;

	assert(A >= 0 && B >= 0);
	if (C < 0) return 0;

	assert(A > 0 && B > 0);
	if (A > B) std::swap(A, B);
	assert(A <= B);

	ll ans = 0;
	while (C >= 0) {
		assert(0 < A && A <= B);

		ll k = B/A;
		ll l = B%A;
		assert(B == k * A + l);

		ll f = C/B;
		ll e = C%B / A;
		ll g = C%B % A;
		assert(C == f * B + e * A + g);
		assert(C == (f * k + e) * A + f * l + g);

		// either x + ky <= f*k+e
		// i.e. 0 <= x <= (f-y) * k + e
		// or x >= fk + e + 1 - ky
		// and Ax + (Ak+l) y <= C = (fk + e + 1) A + fl - A + g
		// Let z = x - (fk + e + 1 - ky)
		// Az + A(fk + e + 1 - ky) + Aky + ly <= C = A (fk + e + 1) + fl - A + g
		// Az + ly <= fl - A + g

		ans += (f+1) * (e+1) + (f+1) * f / 2 * k;

		C = f*l - A + g;
		B = A;
		A = l;
	}
	return ans;
}

// count the number of 0 <= (a * x % m) < c for 0 <= x < n
long long mod_count(long long a, long long m, long long c, long long n) {
	assert(m > 0);
	if (n == 0) return 0;

	a %= m; if (a < 0) a += m;

	long long extraC = c / m; c %= m;
	if (c < 0) extraC--, c += m;
	assert(0 <= c && c < m);

	long long ans = extraC * n;

	long long extraN = n / m; n %= m;
	if (n < 0) extraN--, n += m;
	assert(0 <= n && n < m);

	if (extraN) {
		ans += extraN * (lattice_cnt(m, a+m, (a+m) * (m-1)) - lattice_cnt(m, a+m, (a+m) * (m-1) - c));
	}

	if (n) {
		// we want solutions to 0 <= a(N-1-x) - my < c with 0 <= x <= N-1
		// a * (N-1) >= ax + my > a * (N-1) - c
		ans += lattice_cnt(m, a+m, (a+m) * (n-1)) - lattice_cnt(m, a+m, (a+m) * (n-1) - c);
	}

	return ans;
}

long long mod_count_range(long long a, long long m, long long clo, long long chi, long long nlo, long long nhi) {
	return mod_count(a, m, chi, nhi) - mod_count(a, m, chi, nlo) - mod_count(a, m, clo, nhi) + mod_count(a, m, clo, nlo);
}

using ll = long long;

const ll INF = 8e18;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	ll W, H; cin >> W >> H;

	int N; cin >> N;
	vector<pair<ll, ll>> pts(N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i].first >> pts[i].second;
	}

	ll minDiff = H;
	ll maxDiff = -W;
	ll minSum = H+W; 
	ll maxSum = 0;
	for (auto p : pts) {
		ll x = p.first, y = p.second;

		ll d = y-x;
		assert(-W < d && d < H);
		setmin(minDiff, d);
		setmax(maxDiff, d);

		ll s = x+y;
		assert(0 < s && s < H+W);
		setmin(minSum, s);
		setmax(maxSum, s);
	}

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		ll x, y; cin >> x >> y;
		ll t = [&]() -> ll {
			ll d = y-x;
			// + 2*(W-H) every time

			auto testVal = [&](ll k) -> ll {
				ll ans = -1;
				for (int z = 0; z < N; z++) {
					for (int fx = 0; fx < 2; fx++) {
						for (int fy = 0; fy < 2; fy++) {
							auto p1 = pts[z], p2 = pts[(z+1) % N];
							if (fx) {
								p1.first = 2*W-p1.first;
								p2.first = 2*W-p2.first;
							}
							if (fy) {
								p1.second = 2*H-p1.second;
								p2.second = 2*H-p2.second;
							}
							if (p1.second - p1.first > p2.second - p2.first) swap(p1, p2);

							p1.second += k * (2 * H);
							p2.second += k * (2 * H);

							if (p2.second - p2.first < d) continue;
							ll a = (p2.second - p2.first - d) / (2 * (W+H));
							p2.second -= a * (2 * H);
							p2.first += a * (2 * W);
							assert(p2.second - p2.first >= d);

							p1.second -= a * (2 * H);
							p1.first += a * (2 * W);
							if (p1.second - p1.first > d) continue;
							ll diff = (p2.second - p2.first - d);
							auto p = p2;

							if (p1.second < p2.second) p.second -= diff;
							else if (p1.first > p2.first) p.first += diff;
							else assert(false);

							assert(p.second - p.first == d);
							assert(p1.second <= p.second && p.second <= p2.second);
							assert(p1.first >= p.first && p.first >= p2.first);

							ll cnd = p.second - y;
							if (cnd < 0) continue;
							assert(cnd != 0);
							if (ans == -1 || cnd < ans) ans = cnd;
						}
					}
				}
				return ans;
			};

			ll t0 = testVal(0);
			if (t0 != -1) return t0;

			auto testPref = [&](ll a) -> bool {
				assert(a >= 1);
				auto checkRange = [&](ll lo, ll hi) -> bool {
					return mod_count_range(-2*H, 2*(W+H), lo-d, hi-d + 1, 1, a+1);
				};
				return
					checkRange(minDiff, maxDiff) ||
					checkRange(2*(H-W)-maxDiff, 2*(H-W)-minDiff) ||
					checkRange(-2*W+minSum, -2*W+maxSum) ||
					checkRange(2*H-maxSum, 2*H-minSum);
			};

			ll mi = 0, ma = 2*(W+H);
			if (!testPref(ma)) return -1;

			while (ma - mi > 1) {
				ll md = (mi+ma) / 2;
				if (testPref(md)) ma = md;
				else mi = md;
			}

			ll ans = testVal(ma);
			assert(ans != -1);
			return ans;
		}();

		if (t == -1) {
			cout << -1 << '\n';
		} else {
			ll nx = x + t, ny = y + t;
			nx %= 2*W;
			ny %= 2*H;
			nx = min(nx, 2*W-nx);
			ny = min(ny, 2*H-ny);
			cout << t << ' ' << nx << ' ' << ny << '\n';
		}
	}

	return 0;
}
