#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	int st = 0;
	while (st < N && ((A[st] & 1) == (st & 1))) st++;

	std::vector<int> res; res.reserve(N);
	std::vector<int> end_res; end_res.reserve(N);
	for (int i = st; i < N; ) {
		int v = (A[i] & 1);
		while (i < N && (A[i] & 1) == v) {
			res.push_back(A[i]);
			i++;
		}
		end_res.push_back(res.back());
		res.pop_back();
	}

	std::reverse(end_res.begin(), end_res.end());
	res.insert(res.end(), end_res.begin(), end_res.end());
	cout << res.size() << '\n';
	for (int i = 0; i < int(res.size()); i++) {
		cout << res[i] << " \n"[i+1==int(res.size())];
	}

	return 0;
}
