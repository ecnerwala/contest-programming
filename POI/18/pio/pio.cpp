#include<bits/stdc++.h>

int main() {
	using namespace std;

	int N; scanf("%d", &N);
	vector<int> H(N); for (auto& h : H) scanf("%d", &h);
	vector<int> P = H;

	vector<int> ceil_rt(N);
	for (int i = 0, v = 0; i < N; i++) {
		ceil_rt[i] = v;
		if (v * v == i) v++;
	}

	using pole_t = pair<int, int>;
	vector<pair<pole_t, int>> dq(N);

	for (int flip = 0; flip < 2; flip++, reverse(H.begin(), H.end()), reverse(P.begin(), P.end())) {
		auto sq = [](int64_t a) -> int64_t { return a * a; };
		auto ceil = [](int64_t a, int64_t b) -> int64_t { return (-a) / (-b); };

		auto get_cover = [&](const pole_t& a, const pole_t& b) -> int64_t {
			int d = b.first - a.first;
			int64_t h = b.second - a.second;
			if(h <= 0) return N;
			h *= h;
			if(h >= d) return b.first;

			return b.first + ceil(sq(d - h), 4 * h);
		};

		dq[0].first = {0, H[0]};
		int lo = 0, hi = 1;
		int cur_ceil_rt = 0;
		for (int i = 1; i < N; i++) {
			pole_t cur(i, H[i]);
			int64_t x = get_cover(dq[hi-1].first, cur);
			while (hi - lo >= 2 && x <= dq[hi-1].second) {
				--hi;
				x = get_cover(dq[hi-1].first, cur);
			}
			if (x < N) {
				dq[hi++] = {cur, x};
			}

			while (hi - lo >= 2 && i >= dq[lo+1].second) {
				lo++;
			}

			int d = i - dq[lo].first.first;
			if (d) {
				if (cur_ceil_rt * cur_ceil_rt < d) cur_ceil_rt++;
				while ((cur_ceil_rt-1)*(cur_ceil_rt-1) >= d) cur_ceil_rt --;
				P[i] = max(P[i], cur_ceil_rt + dq[lo].first.second);
			} else {
				cur_ceil_rt = 0;
			}
		}
	}

	for (int i = 0; i < N; i++) {
		printf("%d\n", int(P[i]-H[i]));
	}

	return 0;
}
