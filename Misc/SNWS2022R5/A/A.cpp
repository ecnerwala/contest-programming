#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int A, B; cin >> A >> B;
	A %= B;
	int N; cin >> N;
	string S; cin >> S;
	int K; cin >> K;

	std::vector<int> w(N);
	w[N-1] = 1;
	for (int i = N-2; i >= 0; i--) {
		w[i] = int(int64_t(w[i+1]) * int64_t(A) % B);
	}

	int target_sum = -1;

	{
		string curS = S;
		int cur_val = 0;
		for (int i = 0; i < N; i++) {
			cur_val = (cur_val + w[i] * (curS[i] - 'a')) % B;
		}

		std::vector<int> cnts(B);

		while (true) {
			cnts[cur_val]++;
			if (cnts[cur_val] == K) {
				target_sum = cur_val;
				break;
			}
			{
				// decrement
				int i = N-1;
				while (i >= 0 && curS[i] == 'z') {
					curS[i] = 'a';
					cur_val = (cur_val - w[i] * ('z' - 'a')) % B;
					if (cur_val < 0) cur_val += B;
					i--;
				}
				if (i == -1) {
					cout << -1 << '\n';
					exit(0);
				}
				assert(curS[i] < 'z');
				curS[i]++;
				cur_val += w[i];
				if (cur_val >= B) cur_val -= B;
			}
		}
	}

	assert(target_sum != -1);

	{
		string curS = S;
		int cur_val = 0;
		for (int i = 0; i < N; i++) {
			cur_val = (cur_val + w[i] * (curS[i] - 'a')) % B;
		}

		int need = K;
		while (true) {
			if (cur_val == target_sum) {
				cout << curS << '\n';
				--need;
				if (need == 0) break;
			}
			{
				// decrement
				int i = N-1;
				while (i >= 0 && curS[i] == 'z') {
					curS[i] = 'a';
					cur_val = (cur_val - w[i] * ('z' - 'a')) % B;
					if (cur_val < 0) cur_val += B;
					i--;
				}
				if (i == -1) {
					assert(false);
					cout << -1 << '\n';
					exit(0);
				}
				assert(curS[i] < 'z');
				curS[i]++;
				cur_val += w[i];
				if (cur_val >= B) cur_val -= B;
			}
		}
	}

	return 0;
}
