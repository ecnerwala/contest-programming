#include <bits/stdc++.h>

int eval(const std::vector<int>& P, const std::vector<int>& Q, int K) {
	int N = int(Q.size());
	std::vector<int> invQ(N);
	for (int i = 0; i < N; i++) invQ[Q[i]] = i;

	int ans = 0;
	for (int i = 0; i < N; i++) {
		if (i == K) continue;
		if (invQ[P[Q[i]]] > i) ans++;
	}
	return ans;
}

void test() {
	for (int N = 4; N <= 6; N++) {
		std::vector<std::vector<int>> Ps;
		std::vector<std::vector<int>> Qs;
		{
			std::vector<int> P(N);
			std::iota(P.begin(), P.end(), 0);
			do {
				Qs.push_back(P);
				bool is_derrangement = true;
				for (int i = 0; i < N; i++) { if (P[i] == i) is_derrangement = false; }
				if (is_derrangement) Ps.push_back(P);
			} while (std::next_permutation(P.begin(), P.end()));
		}
		for (int K = 0; K < N; K++) {
			std::map<std::vector<int>, std::vector<int>> mp;
			for (const auto& p : Ps) {
				std::vector<int> results; results.reserve(Qs.size());
				for (const auto& q : Qs) {
					results.push_back(eval(p, q, K));
				}
				mp.insert({results, p});
			}
			if (mp.size() < Ps.size()) {
				continue;
			}
			std::cerr << "good " << N << ' ' << K << '\n';
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	test();
	return 0;
}
