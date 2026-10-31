#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N;
	while (cin >> N) {
		vector<string> C(N); for (auto& c : C) cin >> c;

		deque<int> A;
		int cur = 0;
		A.push_back(1);

		for (int i = 2; i < N; i++) {
			if (C[cur][i] == '1') {
				if (C[A.front()][i] == '1') {
					A.push_back(cur);
					A.push_back(i);
					cur = A.front();
					A.pop_front();
				} else {
					A.push_back(cur);
					cur = i;
				}
			} else {
				if (C[i][A.back()] == '0') {
					A.push_front(cur);
					A.push_front(i);
					cur = A.back();
					A.pop_back();
				} else {
					A.push_front(cur);
					cur = i;
				}
			}
		}
		A.push_front(cur);
		for (int i = 0; i < N; i++) {
			cout << A.front()+1 << " \n"[i+1==N];
			A.pop_front();
		}
	}

	return 0;
}
