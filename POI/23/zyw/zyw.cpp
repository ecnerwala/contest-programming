#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	const int V = (N+1) * (M+1);
	vector<int> par(V, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	auto v = [&](int i, int j) -> int { return i * (M+1) + j; };

	for (int i = 0; i < N; i++) {
		merge(v(i,0), v(i+1,0));
		merge(v(i,M), v(i+1,M));
	}
	for (int j = 0; j < M; j++) {
		merge(v(0,j), v(0,j+1));
		merge(v(N,j), v(N,j+1));
	}

	vector<string> resp0(N, string(M-1,'.'));
	vector<string> resp1(N-1, string(M,'.'));

	int ans = 0;
	for (int i = 0; i < N; i++) {
		string S; cin >> S;
		for (int j = 1; j <= M-1; j++) {
			if (S[j-1] == 'C') {
				if (merge(v(i, j), v(i+1, j))) {
					resp0[i][j-1] = 'Z';
					ans++;
				}
			}
		}
	}
	for (int i = 1; i < N; i++) {
		string S; cin >> S;
		for (int j = 0; j < M; j++) {
			if (S[j] == 'C') {
				if (merge(v(i, j), v(i, j+1))) {
					resp1[i-1][j] = 'Z';
					ans++;
				}
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 1; j <= M-1; j++) {
			if (merge(v(i, j), v(i+1, j))) {
				resp0[i][j-1] = 'Z';
			}
		}
	}
	for (int i = 1; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (merge(v(i, j), v(i, j+1))) {
				resp1[i-1][j] = 'Z';
			}
		}
	}

	cout << (N-1) * (M-1) << ' ' << ans << '\n';
	for (const auto& s : resp0) cout << s << '\n';
	for (const auto& s : resp1) cout << s << '\n';

	return 0;
}
