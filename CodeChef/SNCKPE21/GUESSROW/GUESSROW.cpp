#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T, N; cin >> T >> N;
	bool TESTING;
	if (!cin) {
		T = 500, N = 50;
		cerr << "TESTING!" << '\n';
		TESTING = true;
	} else {
		assert(T == 500);
		assert(N == 50);
		TESTING = false;
	}
	assert(N % 2 == 0);

	std::vector<std::vector<double>> p_win(N);
	{
		std::vector<std::vector<double>> probs(1, std::vector<double>(N, 1.));
		for (int l = 0; l < N; l++) {
			p_win[l].resize(l+1);
			for (int v = 0; v <= l; v++) {
				double tot = 0;
				for (int i = 0; i < N; i++) {
					tot += probs[v][i];
				}
				p_win[l][v] = probs[v][N/2-1] / tot;
			}
			std::vector<std::vector<double>> nprobs(l+2, std::vector<double>(N, 0));
			for (int v = 0; v <= l; v++) {
				for (int i = 0; i < N; i++) {
					// i+1 1's
					int n1 = i+1 - v;
					int n0 = (N-i-1) - (l-v);
					nprobs[v][i] += probs[v][i] * n0 / double(n1+n0);
					nprobs[v+1][i] += probs[v][i] * n1 / double(n1+n0);
				}
			}
			probs = nprobs;
		}
	}

	mt19937 mt(48);
	int num_queries = 0;
	auto query = [&](int i, int j) -> int {
		if (TESTING) {
			num_queries++;
			return j <= i ? 1 : 0;
		} else {
			cout << '?' << ' ' << i+1 << ' ' << j+1 << '\n' << flush;
			int res; cin >> res; return res;
		}
	};
	auto answer = [&](int r) -> void {
		if (TESTING) {
			assert(r+1 == N/2);
		} else {
			cout << '!' << ' ' << r+1 << '\n' << flush;
		}
	};

	for (int round = 1; round <= T; round++) {
		if (TESTING) {
			cerr << "round " << round << '\n';
		}
		std::vector<int> R(N);
		std::iota(R.begin(), R.end(), 0);

		std::vector<std::vector<int>> A(N, std::vector<int>(N));
		for (int i = 0; i < N; i++) {
			std::iota(A[i].begin(), A[i].end(), 0);
			std::shuffle(A[i].begin(), A[i].end(), mt);
		}

		std::vector<int> cnt(N, 0);
		std::vector<int> num(N, 0);
		auto eval = [&](int i) {
			int c = cnt[i];
			int n = num[i];

			assert(n < N);
			assert(0 <= c && c <= n);

			return -p_win[n][c];
		};
		min_pq<std::pair<decltype(eval(0)), int>> pq;
		for (int i = 0; i < N; i++) pq.push({eval(i), i});
		while (true) {
			int i = pq.top().second; pq.pop();
			cnt[i] += query(R[i], A[i].back());
			num[i]++;
			A[i].pop_back();
			if (A[i].empty()) {
				if (cnt[i] == N/2) {
					answer(R[i]);
					goto done;
				}
			} else {
				pq.push({eval(i), i});
			}
		}
done:
		if (TESTING) {
			cerr << double(num_queries) / round << ' ' << num_queries << '\n';
		}
	}

	return 0;
}
