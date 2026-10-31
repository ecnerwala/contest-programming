#include<bits/stdc++.h>

template <typename T, int MAX_K> struct k_max {
	const int k;
	int sz;
	std::array<T, MAX_K * 2> vals;
	k_max(int k_) : k(k_), sz(0) {}

	void trim() {
		assert(sz >= k);
		std::nth_element(vals.begin(), vals.begin() + k-1, vals.begin() + sz, std::greater<T>());
		sz = k;
	}
	void add(const T& t) {
		vals[sz++] = t;
		if (sz == 2 * k) {
			trim();
		}
	}
	void finish() {
		trim();
		std::sort(vals.begin(), vals.begin()+sz, std::greater<T>());
	}
	T operator [] (int a) {
		assert(0 <= a && a < k);
		return vals[a];
	}
};

const int64_t D = 360e9;
struct circ {
	int64_t d;
	int64_t l;
};

int64_t R;
inline double chord_len(int64_t deg) {
	return 2 * double(R) * std::sin(double(deg) / (D/2) * M_PI / 2);
}
inline double tangent_angle(const circ& a, const circ& b) {
	assert(a.d <= b.d);
	double chord = chord_len(b.d - a.d);
	return double(a.d + b.d) / 2 + (std::asin(double(a.l - b.l) / chord) / M_PI * double(D/2));
}
inline double cord_val(const circ& a, const circ& b) {
	return chord_len(std::abs(a.d - b.d)) + double(a.l + b.l);
}
inline bool covers(const circ& a, const circ& b) {
	return double(a.l - b.l) >= chord_len(std::abs(a.d - b.d));
}

std::pair<double, std::array<int, 2>> solve_circles(std::vector<circ> circles) {
	assert(int(circles.size()) >= 2);

	std::pair<double, std::array<int, 2>> best_cord({-1., {-1, -1}});

	std::vector<circ> hull(int(circles.size()));
	std::vector<int> hull_ind(int(circles.size()));
	std::vector<double> tangents(int(circles.size())+1);

	auto check_hull = [&](int i, int j) {
		best_cord = max(best_cord, {cord_val(hull[i], hull[j]), {hull_ind[i], hull_ind[j]}});
	};

	int lo = 0, hi = 0;
	for (int i = 0; i < int(circles.size()); i++) {
		hull[hi] = circles[i];
		hull_ind[hi] = i;
		hi++;
		while (true) {
			if (hi < 2) break;
			check_hull(hi-2, hi-1);

			if (covers(hull[hi-2], hull[hi-1])) {
				hi--;
				break;
			}
			if (covers(hull[hi-1], hull[hi-2])) {
				hull[hi-2] = hull[hi-1];
				hull_ind[hi-2] = hull_ind[hi-1];
				hi--;
				continue;
			}
			tangents[hi-1] = tangent_angle(hull[hi-2], hull[hi-1]);

			if (hi < 3) break;

			if (tangents[hi-2] >= tangents[hi-1]) {
				// delete hi-2
				hull[hi-2] = hull[hi-1];
				hull_ind[hi-2] = hull_ind[hi-1];
				hi--;
				continue;
			}

			break;
		}
	}
	while (true) {
		if (hi - lo < 2) break;
		check_hull(lo, hi-1);
		if (covers(hull[hi-1], hull[lo])) {
			lo++;
			continue;
		}
		if (covers(hull[lo], hull[hi-1])) {
			hi--;
			continue;
		}

		hull[hi-1].d -= D;
		tangents[lo] = tangent_angle(hull[hi-1], hull[lo]);
		hull[hi-1].d += D;

		tangents[hi] = tangents[lo] + D;

		if (tangents[lo] >= tangents[lo+1]) {
			lo++;
			continue;
		}
		if (tangents[hi-1] >= tangents[hi]) {
			hi--;
			continue;
		}
		break;
	}

	assert(lo < hi);

	for (int i = lo, j = lo; j < hi; ) {
		// hardcode this case to avoid precision shenanigans
		if (i == j) {
			j++;
			continue;
		}
		assert(i < j);

		check_hull(i, j);

		if (tangents[j+1] - tangents[i+1] <= double(D/2)) {
			j++;
		} else {
			i++;
		}
	}

	return best_cord;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, K; cin >> N >> R >> K;
		assert(K <= 10);
		vector<circ> circles(N);
		for (int i = 0; i < N; i++) {
			cin >> circles[i].d >> circles[i].l;
		}

		// Maintain K-max
		k_max<double, 10> ans(K);

		while (circles.size() >= 2) {
			auto best_cord = solve_circles(circles);

			if (ans.sz >= K) {
				ans.trim();
				if (ans[K-1] >= best_cord.first) {
					break;
				}
			}

			int remove_ind = best_cord.second[0];
			circ remove_circ = circles[remove_ind];
			circles.erase(circles.begin() + remove_ind);
			for (circ o : circles) {
				ans.add(cord_val(remove_circ, o));
			}
		}

		ans.finish();

		cout << "Case #" << case_num << ": ";
		for (int i = 0; i < K; i++) {
			cout << fixed << setprecision(10) << ans[i] << " \n"[i+1==K];
		}
	}

	return 0;
}
