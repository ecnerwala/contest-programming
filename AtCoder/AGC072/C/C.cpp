#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t K; cin >> N >> K;
	std::vector<int64_t> freq(N);
	freq[0] = K-1;
	int cur = 0;
	std::string res; res.reserve(2*(N-1));
	for (int l = 2; l <= N; l++) {
		for (int i = l-2; i >= 0; i--) {
			int d = l-1-i;
			int64_t td = (freq[i] / l) * d + std::min<int64_t>(d, freq[i] % l);
			freq[i+1] += freq[i] - td;
			freq[i] = td;
		}
		if (freq[cur] <= freq[cur+1]) {
			res += 'D';
		} else {
			res += 'R';
			cur++;
		}
	}

	for (int i = N-2; i >= 0; i--) {
		res += char('D' ^ 'R' ^ res[i]);
	}
	cout << res << '\n';
	return 0;
}
