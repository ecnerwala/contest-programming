#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <typename T> using max_pq = std::priority_queue<T, std::vector<T>, std::less<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S; cin >> S;

	int64_t K; cin >> K;

	std::vector<int> loc; loc.reserve(S.size());
	for (int i = 0; i < int(S.size()); i++) {
		if (S[i] == 'Y') loc.push_back(i);
	}
	int N = int(loc.size());
	for (int i = 0; i < N; i++) {
		loc[i] -= i;
	}

	int ans = 0;

	int64_t cur_val = 0; // sum(his) - sum(los);

	for (int i = 0, j = 0; true; ) {
		if (cur_val <= K) {
			ans = std::max(ans, j-i);
			// increment j
			if (j == N) {
				break;
			}
			cur_val += loc[j];
			cur_val -= loc[(i+j)/2];
			j++;
		} else {
			// increment i
			assert(i < j);
			cur_val += loc[i];
			cur_val -= loc[(i+j)/2];
			i++;
		}
	}

	cout << ans << '\n';

	return 0;
}
