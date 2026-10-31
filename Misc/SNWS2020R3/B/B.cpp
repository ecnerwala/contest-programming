#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> V;
	for (int i = 0; i < (1<<N); i++) {
		int a; cin >> a;
		V.push_back(a);
	}
	sort(V.begin(), V.end());
	if (V.count(0) == 0) {
		cout << "impossible" << '\n';
		exit(0);
	}
	V.erase(V.find(0));
	vector<int> A;
	for (int i = 0; i < N; i++) {
		int cur = *V.begin();
		V.erase(V.begin());
		A.push_back(cur);

		if (cur == 0) {
			auto it = V.begin();
			while (it != V.end()) {
				int v = *it;
				it = V.erase(it);
				if (*it != v) {
					cout << "impossible" << '\n';
					exit(0);
				}
				++it;
			}
		} else {
			auto it = V.begin();
			while (it != V.end()) {
				auto jt = V.find(*it+cur);
				if (jt == V.end()) {
					cout << "impossible" << '\n';
					exit(0);
				}
				V.erase(jt);
				++it;
			}
		}
	}

	for (int i = 0; i < N; i++) {
		cout << A[i] << '\n';
	}

	return 0;
}
