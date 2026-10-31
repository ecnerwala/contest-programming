#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; int64_t R; int K; cin >> N >> R >> K;
		assert(K <= 10);
		struct circ { int64_t d, l; };
		vector<circ> circles(N);
		for (int i = 0; i < N; i++) {
			cin >> circles[i].d >> circles[i].l;
		}

		// Maintain K-max
		vector<double> ans; ans.reserve(2*K);
		auto trim_ans = [&]() {
			assert(int(ans.size()) >= K);
			std::nth_element(ans.begin(), ans.begin()+K, ans.end(), std::greater<>());
			ans.resize(K);
		};
		auto add_cnd = [&](double v) {
			ans.push_back(v);
			if (int(ans.size()) == 2*K) trim_ans();
		};

		const int64_t D = 360e9;
		auto circ_dist = [R](int64_t deg) -> double {
			return 2 * double(R) * sin(double(deg) / (D/2) * M_PI / 2);
		};
		auto diff = [](int64_t a, int64_t b) -> int64_t { return abs(a-b); };
		auto circ_tangent = [](const circ& a, const circ& b, double chord) -> double {
			assert(a.d <= b.d);
			return double(a.d + b.d) / 2 + (asin(double(a.l - b.l) / chord) / M_PI * double(D/2));
		};

		for (int z = 0; z < K && int(circles.size()) >= 2; z++) {
			assert(!circles.empty());

			pair<double, array<int, 2>> best_cord({-1., {-1, -1}});

			vector<int> hull(int(circles.size()));
			vector<double> tangents(int(circles.size())+1);
			int hi = 0;
			for (int i = 0; i < int(circles.size()); i++) {
				hull[hi++] = i;
				while (true) {
					if (hi < 2) break;
					double chord = circ_dist(circles[hull[hi-1]].d - circles[hull[hi-2]].d);
					best_cord = max(best_cord, {
						chord + double(circles[hull[hi-2]].l + circles[hull[hi-1]].l),
						{hull[hi-2], hull[hi-1]}
					});
					if (double(circles[hull[hi-2]].l - circles[hull[hi-1]].l) >= chord) {
						// we're too small
						hi--;
						break;
					}
					if (double(circles[hull[hi-1]].l - circles[hull[hi-2]].l) >= chord) {
						hull[hi-2] = hull[hi-1];
						hi--;
						continue;
					}
					tangents[hi-1] = circ_tangent(circles[hull[hi-2]], circles[hull[hi-1]], chord);

					if (hi < 3) break;

					if (tangents[hi-2] >= tangents[hi-1]) {
						// delete hi-2
						hull[hi-2] = hull[hi-1];
						hi--;
						continue;
					}

					break;
				}
			}

			int lo = 0;
			while (true) {
				if (hi - lo < 2) break;
				double chord = circ_dist(circles[hull[lo]].d + D - circles[hull[hi-1]].d);
				best_cord = max(best_cord, {
					chord + double(circles[hull[lo]].l + circles[hull[hi-1]].l),
					{hull[lo], hull[hi-1]}
				});
				if (double(circles[hull[hi-1]].l - circles[hull[lo]].l) >= chord) {
					lo++;
					continue;
				}
				if (double(circles[hull[lo]].l - circles[hull[hi-1]].l) >= chord) {
					hi--;
					continue;
				}

				circles[hull[hi-1]].d -= D;
				tangents[lo] = circ_tangent(circles[hull[hi-1]], circles[hull[lo]], chord);
				circles[hull[hi-1]].d += D;

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

				if (i < j) {
					best_cord = max(best_cord, {
						circ_dist(circles[hull[j]].d - circles[hull[i]].d) + double(circles[hull[i]].l + circles[hull[j]].l),
							{hull[i], hull[j]}
					});
				}
				if (tangents[j+1] - tangents[i+1] <= double(D/2)) {
					j++;
				} else {
					i++;
				}
			}

			int remove_ind = best_cord.second[0];
			circ remove_circ = circles[remove_ind];
			circles.erase(circles.begin() + remove_ind);
			for (circ o : circles) {
				add_cnd(circ_dist(diff(o.d, remove_circ.d)) + double(o.l + remove_circ.l));
			}
		}

		assert(int(ans.size()) >= K);
		trim_ans();
		sort(ans.begin(), ans.end(), std::greater<>());
		cout << "Case #" << case_num << ": ";
		for (int i = 0; i < K; i++) {
			cout << fixed << setprecision(10) << ans[i] << " \n"[i+1==K];
		}
	}

	return 0;
}
