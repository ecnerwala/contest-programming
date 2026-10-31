#include <bits/extc++.h>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

int conta(int N, std::vector<int> &A, std::vector<int> &B, std::vector<int> &C) {
	hash_map<int, int> Aloc;
	for (int i = 0; i < int(A.size()); i++) {
		if (Aloc.find(A[i]) != Aloc.end()) {
			A.resize(i);
			break;
		}
		Aloc[A[i]] = i;
	}
	hash_map<int, int> Bloc;
	for (int i = 0; i < int(B.size()); i++) {
		if (Bloc.find(B[i]) != Bloc.end()) {
			B.resize(i);
			break;
		}
		Bloc[B[i]] = i;
	}
	hash_map<int, int> Cloc;
	for (int i = 0; i < int(C.size()); i++) {
		if (Cloc.find(C[i]) != Cloc.end()) {
			C.resize(i);
			break;
		}
		Cloc[C[i]] = i;
	}

	std::vector<int> maxC(int(B.size())+1);
	maxC[0] = int(C.size());

	for (int i = 0; i < int(B.size()); i++) {
		int v = maxC[i];
		auto it = Cloc.find(B[i]);
		if (it != Cloc.end()) {
			v = std::min(v, it->second);
		}
		maxC[i+1] = v;
	}

	std::set<std::pair<int, int>> best_BC;
	for (int i = 0; i <= int(B.size()); i++) {
		best_BC.insert({i + maxC[i], i});
	}

	int ans = best_BC.rbegin()->first;
	int curB = int(B.size());
	int curC = int(C.size());
	int loB = 0;
	for (int i = 0; i < int(A.size()); i++) {
		if (Bloc.find(A[i]) != Bloc.end()) curB = std::min(curB, Bloc[A[i]]);
		if (Cloc.find(A[i]) != Cloc.end()) curC = std::min(curC, Cloc[A[i]]);

		loB = std::min(loB, curB+1);
		while (loB <= curB && maxC[loB] >= curC) {
			loB++;
		}
		assert(1 <= loB && loB <= curB+1);
		int v = curB + std::min(curC, maxC[curB]);
		v = std::max(v, loB-1 + curC);
		while (!best_BC.empty() && (best_BC.rbegin()->second > curB || maxC[best_BC.rbegin()->second] > curC)) {
			best_BC.erase(--best_BC.end());
		}
		if (!best_BC.empty()) v = std::max(v, best_BC.rbegin()->first);
		ans = std::max(ans, i + 1 + v);
	}

	return ans;
}
