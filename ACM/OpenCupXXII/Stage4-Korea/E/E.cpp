#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; int64_t P; cin >> N >> K >> P;

	std::vector<std::pair<int64_t, int64_t>> C(N);
	for (auto& it : C) cin >> it.first >> it.second;

	if (P % C[0].first != 0) {
		cout << -1 << '\n';
		exit(0);
	}

	const int64_t INF = 2e18;
	using vect = std::vector<std::pair<int64_t, int64_t>>;

	auto mult = [&](const vect& a, const vect& b) -> vect {
		vect r(K+1, {+INF, -INF});
		for (int i = 0; i <= K; i++) {
			if (a[i].first > a[i].second) continue;
			for (int j = 0; j <= K-i; j++) {
				if (b[j].first > b[j].second) continue;
				setmin(r[i+j].first, a[i].first + b[j].first);
				setmax(r[i+j].second, a[i].second + b[j].second);
			}
		}
		return r;
	};

	auto pow = [&](vect a, int64_t b, vect r) -> vect {
		while (b) {
			if (b & 1) r = mult(r, a);
			b >>= 1;
			if (b) {
				a = mult(a, a);
			}
		}
		return r;
	};

	vect ID(K+1, {+INF, -INF});
	ID[0] = {0,0};

	vect res = ID;

	vect cur(K+1, {+INF, -INF});

	for (int i = 0; i < N; i++) {
		setmin(cur[1].first, C[i].second);
		setmax(cur[1].second, C[i].second);
		int64_t cnt = ((i+1 < N) ? (P % C[i+1].first) : P) / C[i].first;
		if (cnt > K) {
			cout << -1 << '\n';
			exit(0);
		}
		res = pow(cur, cnt, res);
		if (i+1 < N) {
			cur = pow(cur, C[i+1].first / C[i].first, ID);
		}
	}

	int64_t lo = res[K].first;
	int64_t hi = res[K].second;
	if (lo <= hi) {
		cout << lo << ' ' << hi << '\n';
	} else {
		cout << -1 << '\n';
	}

	return 0;
}
