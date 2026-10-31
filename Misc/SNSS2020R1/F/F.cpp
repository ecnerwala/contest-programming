#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	while (true) {
		int N, K; cin >> N >> K;
		if (N == 0 && K == 0) break;
		assert(N != 0 && K != 0);

		vector<array<int, 2>> pts(N);
		for (auto& pt : pts) cin >> pt[0] >> pt[1];
		sort(pts.begin(), pts.end());
		auto area = [](const array<int, 2>& a, const array<int, 2>& b, const array<int, 2>& c) -> int {
			return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
		};
		auto signum = [](int a) -> int { return (a > 0) - (a < 0); };

		vector<array<int, 2>> upper_hull; upper_hull.reserve(N);
		vector<array<int, 2>> lower_hull; lower_hull.reserve(N);
		for (const auto& pt : pts) {
			while (upper_hull.size() >= 2) {
				auto b = upper_hull.back(); upper_hull.pop_back();
				if (area(upper_hull.back(), b, pt) < 0) {
					upper_hull.push_back(b);
					break;
				}
			}
			upper_hull.push_back(pt);

			while (lower_hull.size() >= 2) {
				auto b = lower_hull.back(); lower_hull.pop_back();
				if (area(lower_hull.back(), b, pt) > 0) {
					lower_hull.push_back(b);
					break;
				}
			}
			lower_hull.push_back(pt);
		}

		vector<array<int, 2>> hull;
		hull.insert(hull.end(), lower_hull.begin(), lower_hull.end());
		hull.insert(hull.end(), upper_hull.rbegin()+1, upper_hull.rend());
		assert(hull.front() == hull.back());

		auto is_simple = [&area, &signum](const vector<array<int, 2>>& v) -> bool {
			assert(v.front() == v.back());
			int L = int(v.size()) - 1;
			for (int i = 0; i < L; i++) {
				for (int j = i+1; j < L; j++) {
					if (v[i] == v[j]) return false;
				}
			}
			for (int i = 0; i < L; i++) {
				for (int j = i+2; j < L; j++) {
					if (i == 0 && j == L-1) continue;
					if (
						signum(area(v[i], v[i+1], v[j])) * signum(area(v[i], v[i+1], v[j+1])) == -1 &&
						signum(area(v[j], v[j+1], v[i])) * signum(area(v[j], v[j+1], v[i+1])) == -1) {
						return false;
					}
				}
			}
			return true;
		};
		auto can_remove = [&](array<int, 2> a, array<int, 2> b, array<int, 2> c) -> bool {
			assert(area(a, b, c) > 0);
			for (auto p : pts) {
				if (p == a || p == b || p == c) continue;
				if (area(a, b, p) > 0 && area(a, p, c) > 0 && area(p, b, c) > 0) return false;
			}
			return true;
		};
		auto perimeter = [](const vector<array<int, 2>>& v) -> double {
			double ans = 0;
			for (int i = 0; i+1 < int(v.size()); i++) {
				ans += std::hypot(v[i+1][0]-v[i][0], v[i+1][1] - v[i][1]);
			}
			return ans;
		};

		set<pair<double, vector<array<int, 2>>>> polygons;
		assert(is_simple(hull));
		polygons.insert({perimeter(hull), hull});
		while (!polygons.empty() && K > 1) {
			auto v = polygons.begin()->second;
			polygons.erase(polygons.begin());
			K--;
			for (int i = 0; i+1 < int(v.size()); i++) {
				for (auto pt : pts) {
					auto nv = v;
					nv.insert(nv.begin() + i+1, pt);
					if (area(v[i], v[i+1], pt) > 0 && can_remove(v[i], v[i+1], pt) && is_simple(nv)) {
						polygons.insert({perimeter(nv), nv});
					}
				}
			}
		}
		if (polygons.empty()) {
			cout << -1 << '\n';
		} else {
			cout << fixed << setprecision(14) << polygons.begin()->first << '\n';
		}
	}

	return 0;
}
