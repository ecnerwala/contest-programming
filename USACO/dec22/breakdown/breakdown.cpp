#include <bits/stdc++.h>

inline void setmin(int& a, int b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> origW(N*N);
	for (auto& v : origW) cin >> v;

	std::vector<int> edges(N*N);
	for (auto& e : edges) {
		int i, j; cin >> i >> j; i--, j--;
		e = i*N+j;
	}
	std::reverse(edges.begin(), edges.end());

	const int INF = int(1e9)+20;

	std::vector<int> answers; answers.reserve(N*N);
	const int S = 0;
	const int T = N-1;

	std::vector<int> W1(N*N, INF);
	std::vector<int> W2(N*N, INF);
	std::vector<int> WS3(N, INF);
	std::vector<int> WS4(N, INF);
	std::vector<int> WT3(N, INF);
	std::vector<int> WT4(N, INF);
	for (auto e : edges) {
		if (K == 2) {
			answers.push_back(W2[S*N+T]);
		} else if (K == 3) {
			answers.push_back(WS3[T]);
		} else if (K == 4) {
			answers.push_back(WS4[T]);
		} else if (K == 5) {
			int ans = INF;
			for (int i = 0; i < N; i++) {
				setmin(ans, W2[S*N+i] + WT3[i]);
			}
			answers.push_back(ans);
		} else if (K == 6) {
			int ans = INF;
			for (int i = 0; i < N; i++) {
				setmin(ans, WS3[i] + WT3[i]);
			}
			answers.push_back(ans);
		} else if (K == 7) {
			int ans = INF;
			for (int i = 0; i < N; i++) {
				setmin(ans, WS3[i] + WT4[i]);
			}
			answers.push_back(ans);
		} else if (K == 8) {
			int ans = INF;
			for (int i = 0; i < N; i++) {
				setmin(ans, WS4[i] + WT4[i]);
			}
			answers.push_back(ans);
		} else assert(false);

		{
			int i = e / N, j = e % N;
			int w1ij = W1[i*N+j] = origW[i*N+j];

			// Fix W2
			for (int k = 0; k < N; k++) {
				setmin(W2[i*N+k], w1ij + W1[j*N+k]);
				setmin(W2[k*N+j], W1[k*N+i] + w1ij);
			}

			// Fix WS3/WS4
			if (i == S) {
				for (int k = 0; k < N; k++) {
					int v = std::min(w1ij + W2[j*N+k], INF);
					setmin(WS3[k], v);
					for (int l = 0; l < N; l++) {
						setmin(WS4[l], v + W1[k*N+l]);
					}
				}
			}
			{
				int w2Sj = std::min(W1[S*N+i] + w1ij, INF);
				int w3Sj = std::min(W2[S*N+i] + w1ij, INF);
				for (int k = 0; k < N; k++) {
					setmin(WS3[k], w2Sj + W1[j*N+k]);
					setmin(WS4[k], w2Sj + W2[j*N+k]);
					setmin(WS4[k], w3Sj + W1[j*N+k]);
				}
			}
			setmin(WS3[j], W2[S*N+i] + w1ij);
			setmin(WS4[j], WS3[i] + w1ij);

			// Fix WT3/WT4
			if (j == T) {
				for (int k = 0; k < N; k++) {
					int v = std::min(W2[k*N+i] + w1ij, INF);
					setmin(WT3[k], v);
					for (int l = 0; l < N; l++) {
						setmin(WT4[l], W1[l*N+k] + v);
					}
				}
			}
			{
				int w2iT = std::min(w1ij + W1[j*N+T], INF);
				int w3iT = std::min(w1ij + W2[j*N+T], INF);
				for (int k = 0; k < N; k++) {
					setmin(WT3[k], W1[k*N+i] + w2iT);
					setmin(WT4[k], W2[k*N+i] + w2iT);
					setmin(WT4[k], W1[k*N+i] + w3iT);
				}
			}
			setmin(WT3[i], w1ij + W2[j*N+T]);
			setmin(WT4[i], w1ij + WT3[j]);
		}
	}

	std::reverse(answers.begin(), answers.end());
	for (int v : answers) {
		if (v >= INF) cout << -1 << '\n';
		else cout << v << '\n';
	}

	return 0;
}
