#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> C(N);
	for (auto& c : C) cin >> c;

	auto solve = [&]() -> double {
		std::vector<int> freq(N+1, 0);
		struct state_t {
			double val;
			double p;
			double c;
		};
		std::vector<state_t> stk; stk.reserve(N+1);
		stk.push_back({0, -1, -1});
		int tot = 0;
		int64_t tot_C = 0;
		for (int i = N-1; i >= 0; i--) {
			int a = A[i];
			tot_C += C[i];
			freq[a]++;
			tot++;

			double p = double(freq[a]) / double(tot);
			double c = C[i];
			double val = (stk.back().val + c) * (p / (2-p));
			while (int(stk.size()) >= 2) {
				double np = p * stk.back().p;
				double nc = c + stk.back().c;
				double nval = (stk.end()[-2].val + nc) * (np / (2-np));
				if (nval >= val) {
					stk.pop_back();
					p = np;
					c = nc;
					val = nval;
				} else {
					break;
				}
			}
			stk.push_back({val, p, c});
		}
		return (double(tot_C) + stk.back().val) / 2;
	};

	cout << fixed << setprecision(12);
	cout << solve() << '\n';

	int Q; cin >> Q;
	while (Q--) {
		int k; cin >> k;
		while (k--) {
			int p, x, y; cin >> p >> x >> y; p--;
			A[p] = x;
			C[p] = y;
		}
		cout << solve() << '\n';
	}

	return 0;
}
