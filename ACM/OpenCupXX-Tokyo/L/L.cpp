#include<bits/stdc++.h>
using namespace std;

template <typename T> T& setmax(T& a, const T& b) { if (b > a) a = b; return a; }
template <typename T> T& setmin(T& a, const T& b) { if (b < a) a = b; return a; }

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N; cin >> N;

	struct pt {
		int x, y, w;
	};

	vector<pt> A(N);
	for (auto& p : A) cin >> p.x >> p.y >> p.w;
	sort(A.begin(), A.end(), [&](const auto& p, const auto& q) { return p.y < q.y; });

	vector<pt> B(N);
	for (auto& p : B) cin >> p.x >> p.y >> p.w;
	sort(B.begin(), B.end(), [&](const auto& p, const auto& q) { return p.y < q.y; });

	vector<pt> bestA = A;
	{
		int l = 0;
		for (int i = 1; i < N; i++) {
			if (bestA[i].w > bestA[l].w) {
				bestA[++l] = bestA[i];
			}
		}
		bestA.resize(l+1);
	}
	vector<pt> bestB = B;
	{
		int l = N-1;
		for (int i = N-2; i >= 0; i--) {
			if (bestB[i].w > bestB[l].w) {
				bestB[--l] = bestB[i];
			}
		}
		bestB.erase(bestB.begin(), bestB.begin()+l);
	}

	vector<pair<pair<int, int>, int>> cnds; cnds.reserve(int(A.size()) + int(B.size()) + int(bestA.size()) + int(bestB.size()));
	auto addCnd = [&](const pt& a, const pt& b) {
		assert(a.y < b.y);
		cnds.push_back({{a.x, b.x}, a.w + b.w});
	};

	for (int i = 0, j = 0; i < N; i++) {
		while (j < int(bestB.size()) && bestB[j].y < A[i].y) {
			j++;
		}
		if (j == int(bestB.size())) break;
		addCnd(A[i], bestB[j]);
	}
	for (int i = 0, j = 0; i+1 < int(bestA.size()); i++) {
		while (j < int(bestB.size()) && bestB[j].y < bestA[i+1].y) {
			j++;
		}
		if (j == int(bestB.size())) break;
		addCnd(bestA[i], bestB[j]);
	}

	for (int i = int(bestA.size())-1, j = N-1; j >= 0; j--) {
		while (i >= 0 && bestA[i].y > B[j].y) {
			i--;
		}
		if (i == -1) break;
		addCnd(bestA[i], B[j]);
	}

	/*
	for (int i = int(bestA.size())-1, j = int(bestB.size())-1; j > 0; j--) {
		while (i >= 0 && bestA[i].y > bestB[j-1].y) {
			i--;
		}
		if (i == -1) break;
		addCnd(bestA[i], bestB[j]);
	}
	*/

	int Q; cin >> Q;
	vector<pair<pair<int, int>, int>> queries(Q);
	for (int q = 0; q < Q; q++) {
		cin >> queries[q].first.first >> queries[q].first.second;
		queries[q].second = q;
	}

	vector<int> ans(Q, -1);

	vector<int> ys(cnds.size());
	for (int i = 0; i < int(cnds.size()); i++) {
		ys[i] = cnds[i].first.second;
	}
	sort(ys.begin(), ys.end());
	ys.resize(unique(ys.begin(), ys.end()) - ys.begin());

	for (auto& it : cnds) it.first.second = int(lower_bound(ys.begin(), ys.end(), it.first.second) - ys.begin());
	for (auto& it : queries) it.first.second = int(lower_bound(ys.begin(), ys.end(), it.first.second) - ys.begin());

	sort(cnds.begin(), cnds.end());
	sort(queries.begin(), queries.end());

	for (int z = 0; z < 2; z++) {
		vector<int> bit(ys.size(), -1);

		auto cndIt = cnds.begin();
		for (auto q : queries) {
			while (cndIt != cnds.end() && cndIt->first.first < q.first.first) {
				for (int a = cndIt->first.second+1; a; a -= a & -a) {
					setmax(bit[a-1], cndIt->second);
				}
				++cndIt;
			}

			for (int a = q.first.second+1; a <= int(ys.size()); a += a & -a) {
				setmax(ans[q.second], bit[a-1]);
			}
		}

		for (auto& it : cnds) it.first.first *= -1, it.first.second = int(ys.size())-1-it.first.second;
		reverse(cnds.begin(), cnds.end());

		for (auto& it : queries) it.first.first *= -1, it.first.second = int(ys.size())-it.first.second;
		reverse(queries.begin(), queries.end());
	}

	for (int a : ans) cout << a << '\n';

	return 0;
}
