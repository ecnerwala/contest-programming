#include<bits/stdc++.h>
using namespace std;

using i64 = int64_t;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int NA; cin >> NA;
	vector<pair<int, int>> A(NA);
	for (auto& p : A) cin >> p.first >> p.second;
	int VXA, VYA; cin >> VXA >> VYA;

	int NB; cin >> NB;
	vector<pair<int, int>> B(NB);
	for (auto& p : B) cin >> p.first >> p.second;
	int VXB, VYB; cin >> VXB >> VYB;

	int VX = VXA - VXB, VY = VYA - VYB;
	for (auto& p : A) {
		p = {p.first * VX + p.second * VY, p.second * VX - p.first * VY};
	}
	for (auto& p : B) {
		p = {p.first * VX + p.second * VY, p.second * VX - p.first * VY};
	}

	// now we're just moving left->right at a rate (VX^2 + VY^2)
	int V = VX * VX + VY * VY;
	//cerr << "V = " << V << '\n';
	//cerr << VX << ' ' << VY << '\n';

	//cerr << "A" << '\n'; for (auto p : A) { cerr << p.first << ' ' << p.second << '\n'; } cerr << '\n';
	//cerr << "B" << '\n'; for (auto p : B) { cerr << p.first << ' ' << p.second << '\n'; } cerr << '\n';

	A.push_back(A.front());
	B.push_back(B.front());

	vector<pair<double, tuple<double, double, double>>> evts;
	vector<double> emptyEvts;

	for (int i = 0; i < NA; i++) {
		for (int j = 0; j < NB; j++) {
			pair<int, int> a1 = A[i], a2 = A[i+1];
			pair<int, int> b1 = B[j], b2 = B[j+1];
			int w = -1;
			if (a1.second > a2.second) swap(a1, a2), w = -w;
			if (b1.second > b2.second) swap(b1, b2), w = -w;
			assert(a1.second <= a2.second);
			assert(b1.second <= b2.second);
			if (a1.second == a2.second || b1.second == b2.second) {
				// we'll catch it with the endpoint things
				continue;
			}
			assert(a1.second < a2.second);
			assert(b1.second < b2.second);

			int ylo = max(a1.second, b1.second);
			int yhi = min(a2.second, b2.second);
			if (ylo > yhi) continue;

			auto interp = [](const pair<int, int>& plo, const pair<int, int>& phi, int y) -> double {
				return double(phi.first * i64(y - plo.second) + plo.first * i64(phi.second - y)) / double(phi.second - plo.second);
			};

			double tlo = interp(b1, b2, ylo) - interp(a1, a2, ylo);
			double thi = interp(b1, b2, yhi) - interp(a1, a2, yhi);

			double t0 = min(tlo, thi), t1 = max(tlo, thi);

			bool sameSlope = i64(b2.second - b1.second) * i64(a2.first - a1.first) == i64(a2.second - a1.second) * i64(b2.first - b1.first);

			int dy = (yhi - ylo) * w;

			if (ylo == yhi || sameSlope) {
				assert(abs(t1-t0) <= 1e-7);
				if (dy) {
					evts.push_back({t0, {-t0*dy, dy, 0}});
				} else {
					emptyEvts.push_back(t0);
				}
			} else {
				double a = dy / double(t1 - t0);
				// from t0 to t1, value is
				// 1/2 (x - t0)^2 * dy / (t1 - t0)
				evts.push_back({t0, {a * t0 * t0 / 2, -a * t0, a / 2}});
				// from t1 onwards, value is dy (x - (t0 + t1)/2)
				evts.push_back({t1, {-a * t1 * t1 / 2, a * t1, -a / 2}});
			}
		}
	}
	if (evts.empty()) {
		if (emptyEvts.empty()) {
			cout << "never" << '\n';
			return 0;
		} else {
			cout << *min_element(emptyEvts.begin(), emptyEvts.end())/V << '\n';
			return 0;
		}
	}
	sort(evts.begin(), evts.end());
	if (evts.back().first < 0) {
		cout << "never" << '\n';
		return 0;
	}
	for (int i = 0; i+1 < int(evts.size()); i++) {
		get<0>(evts[i+1].second) += get<0>(evts[i].second);
		get<1>(evts[i+1].second) += get<1>(evts[i].second);
		get<2>(evts[i+1].second) += get<2>(evts[i].second);
	}
	auto eval = [&](double d, tuple<double, double, double> q) {
		return (get<2>(q) * d + get<1>(q)) * d + get<0>(q);
	};
	for (int i = 0; i+1 < int(evts.size()); i++) {
		//cerr << evts[i].first << ' ' << eval(evts[i].first, evts[i].second) << '\n';
		//cerr << get<0>(evts[i].second) << ' ' << get<1>(evts[i].second) << ' ' << get<2>(evts[i].second) << '\n';
		//cerr << evts[i+1].first << ' ' << eval(evts[i+1].first, evts[i].second) << '\n';
	}
	//cerr << get<0>(evts.back().second) << ' ' << get<1>(evts.back().second) << ' ' << get<2>(evts.back().second) << '\n';

	//cerr << '\n';

	const double EPS = 1e-9;
	for (int i = 0; i+1 < int(evts.size()); i++) {
		double c0, c1, c2; tie(c0, c1, c2) = evts[i].second;
		double t0 = evts[i].first, t1 = evts[i+1].first;

		double d0 = 2 * c2 * t0 + c1, d1 = 2 * c2 * t1 + c1;
		//cerr << t0 << '-' << t1 << '\n';
		//cerr << c0 << ' ' << c1 << ' ' << c2 << '\n';
		//cerr << eval(t0, {c0,c1,c2}) << ' ' << eval(t1, {c0,c1,c2}) << '\n';
		//cerr << d0 << ' ' << d1 << '\n';

		if (t1 - t0 < EPS) {
			continue;
		}
		if (d0 < EPS && d1 < EPS) {
			cout << fixed << setprecision(10) << t0 / V << '\n';
			return 0;
		}
		if (d1 < EPS) {
			// solve
			double t = -c1 / c2 / 2;
			//cerr << t << ' ' << eval(t, {c0,c1,c2}) << '\n';
			cout << fixed << setprecision(10) << t / V << '\n';
			return 0;
		}
	}

	return 0;
}
