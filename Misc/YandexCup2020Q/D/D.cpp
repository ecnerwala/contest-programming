#include<bits/stdc++.h>
#include <numeric>
#include <queue>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, K, R; cin >> N >> K >> R;
	vector<int64_t> A(N); for (auto& a : A) cin >> a;
	sort(A.begin(), A.end());

	int64_t lo_sum = std::accumulate(A.begin(), A.begin()+K, int64_t(0));
	std::priority_queue<int64_t, vector<int64_t>, greater<int64_t>> los(A.begin(), A.begin()+K);
	std::priority_queue<int64_t, vector<int64_t>, greater<int64_t>> his(A.begin()+K, A.end());
	for (int z = 0; z < R; z++) {
		int64_t nv = lo_sum;
		lo_sum -= los.top(); los.pop();
		his.push(nv);
		lo_sum += his.top(); los.push(his.top()); his.pop();
	}

	for (int z = 0; z < N; z++) {
		int64_t v;
		if (!los.empty()) {
			v = los.top(), los.pop();
		} else {
			v = his.top(), his.pop();
		}
		cout << v << " \n"[z+1==N];
	}

	return 0;
}
