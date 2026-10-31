#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N; int64_t P; cin >> N >> P;

		// team 0 has slightly less penalty
		int64_t p0_minus_p1 = -1;

		std::vector<int64_t> times; times.reserve(N+1);
		times.push_back(0);
		std::vector<int> weights; weights.reserve(N);

		{
			std::array<std::map<int, int>, 2> status;
			for (int i = 0; i < N; i++) {
				int a, b; int64_t c; int d; cin >> a >> b >> c >> d;
				a--;
				auto& v = status[a][b];
				if (v == -1) {
					// already solved, do nothing
				} else if (d == 1) {
					// solve the problem
					int64_t penalty = v * P + c;
					v = -1;
					p0_minus_p1 += 2 * (a ? -penalty : +penalty);
					times.push_back(c);
					weights.push_back(a ? -2 : +2);
				} else if (d == 0) {
					v++;
				} else assert(false);
			}
		}

		if (p0_minus_p1 < 0) {
			p0_minus_p1 = -p0_minus_p1;
			for (auto& w : weights) w = -w;
		}
		int L = int(weights.size());
		assert(int(times.size()) == L+1);
		for (int i = L-1; i > 0; i--) {
			weights[i-1] += weights[i];
		}

		int64_t T = times.back() - times.front();
		std::pair<int64_t, int64_t> best{T+1, -1};

		for (int z = 0; z < 2; z++) {
			std::reverse(times.begin(), times.end());
			for (auto& t : times) t = -t;
			std::reverse(weights.begin(), weights.end());

			for (int i = 0; i < L; i++) { cerr << '@' << times[i] << ' ' << 'x' << weights[i] << ' '; } cerr << '@' << times[L] << '\n';

			for (int i = 0; i < L; i++) {
			}

			if (z == 0) {
				// this is reversed
			}
		}

	}

	return 0;
}
