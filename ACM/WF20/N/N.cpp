#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,fma") // Requires AVX2
#include <bits/stdc++.h>

double sq(double x) { return x * x; }

double norm(const std::vector<double>& v) {
	double r = 0;
	for (auto x : v) r += x * x;
	return r;
}

double abs(const std::vector<double>& v) {
	return std::sqrt(norm(v));
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int D, N; cin >> D >> N;
	std::vector<std::pair<std::vector<double>, double>> circles(N);
	for (auto& [v, r] : circles) {
		v.resize(D);
		for (auto& x : v) cin >> x;
		cin >> r;
		r *= r;
	}

	std::vector<std::vector<double>> other_dirs(N <= D ? 1 : 0, std::vector<double>(D));
	for (int d = 0; d <	int(other_dirs.size()); d++) other_dirs[d][d] = 100;

	for (int iter = 0; true; iter++) {
		if (iter == D) {
			// we've reduced all dimensions, so all points should be equal, and the radius should be eps
			break;
		}
		if (N == 1) {
			double best_norm = -1;
			int best_idx = -1;
			for (int i = 0; i < int(other_dirs.size()); i++) {
				double cnd = norm(other_dirs[i]);
				if (cnd > best_norm) { best_norm = cnd, best_idx = i; }
			}
			assert(best_norm >= 0);
			double scale = std::sqrt(std::max(circles[0].second / best_norm, 0.));
			for (int d = 0; d < D; d++) {
				circles[0].first[d] += other_dirs[best_idx][d] * scale;
			}
			break;
		}

		N--;
		double best_norm = -1;
		int best_idx = -1;
		for (int i = N-1; i < N; i++) {
			double dist = 0;
			for (int d = 0; d < D; d++) {
				dist += sq(circles[i].first[d] - circles[N].first[d]);
			}
			if (dist > best_norm) { best_norm = dist, best_idx = i; }
		}
		std::vector<double> dir(D);
		double target_dot = 0;
		for (int d = 0; d < D; d++) {
			dir[d] = circles[best_idx].first[d] - circles[N].first[d];
			target_dot += (circles[best_idx].first[d] + circles[N].first[d]) / 2 * dir[d];
		}
		target_dot += (circles[N].second - circles[best_idx].second) / 2;

		circles.pop_back();
		for (int i = 0; i < N; i++) {
			double dot = 0;
			for (int d = 0; d < D; d++) {
				dot += circles[i].first[d] * dir[d];
			}

			circles[i].second -= sq(target_dot - dot) / best_norm;
			if (circles[i].second < 0) circles[i].second = 0;

			double scale = (target_dot - dot) / best_norm;
			for (int d = 0; d < D; d++) {
				circles[i].first[d] += scale * dir[d];
			}
		}

		for (auto& v : other_dirs) {
			double dot = 0;
			for (int d = 0; d < D; d++) {
				dot += v[d] * dir[d];
			}
			double scale = -dot / best_norm;
			for (int d = 0; d < D; d++) {
				v[d] += scale * dir[d];
			}
		}
	}

	for (int d = 0; d < D; d++) {
		cout << fixed << setprecision(12) << circles[0].first[d] << " \n"[d+1==D];
	}

	return 0;
}
