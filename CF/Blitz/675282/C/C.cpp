#include <bits/stdc++.h>
int main() {
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
		for (auto a : A) pq.push(a);
		bool good = true;
		while (pq.size() >= 2) {
			int a = pq.top(); pq.pop();
			int b = pq.top(); pq.pop();
			if (std::abs(a - b) > 1) { good = false; break; }
			pq.push(a+b);
		}
		std::cout << (good ? "YES" : "NO") << '\n';
	}
}
