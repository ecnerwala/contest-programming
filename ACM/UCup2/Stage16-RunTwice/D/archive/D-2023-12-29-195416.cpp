#include <bits/stdc++.h>

using std::cerr;

std::vector<bool> string_to_bits(std::string S) {
	std::vector<bool> res; res.reserve(5 * S.size());
	for (char c : S) {
		int v = c - 'a';
		assert(0 <= v && v < 26);
		for (int i = 0; i < 5; i++) {
			res.push_back(bool((v >> i) & 1));
		}
	}
	return res;
}

std::string bits_to_string(std::vector<bool> V) {
	while (int(V.size()) % 5) V.push_back(0);
	std::string S; S.reserve(int(V.size()) % 5);
	for (int i = 0; i < int(V.size()); i += 5) {
		int v = 0;
		for (int z = 0; z < 5; z++) {
			v |= (V[i+z] << z);
		}
		S += char('a' + std::min(v, 25));
	}
	return S;
}

std::vector<int> Alisa(std::string S, std::vector<int> V) {
	//assert(int(V.size()) == 10000);
	auto B = string_to_bits(S);
	int L = int(V.size()) / int(B.size());
	int cur = 0;
	std::vector<int> res; res.reserve(V.size());
	for (int b = 0; b < int(B.size()); b++) {
		// +1 is 1, +2 is 0
		cur = (cur + (B[b] ? 1 : 2)) % 3;
		for (int i = 0; i < L; i++) {
			if (V[b * L + i] % 3 == cur) {
				res.push_back(V[b * L + i]);
			}
		}
	}
	return res;
}

std::string Eva(std::vector<int> V) {
	std::vector<bool> B; B.reserve(75);
	int cur = 0;
	for (int v : V) {
		v %= 3;
		if (v != cur) {
			int d = (v - cur + 3) % 3;
			assert(d == 1 || d == 2);
			B.push_back(d == 1 ? 1 : 0);
			cur = v;
		}
	}
	return bits_to_string(B);
}

void test() {
	std::mt19937 mt(48);
	for (int z = 0; z < 100; z++) {
		std::cerr << "running test " << z << '\n';
		std::vector<int> V(10000);
		for (auto& i : V) i = std::uniform_int_distribution<int>(0, int(1e9)-1)(mt);
		int N = std::uniform_int_distribution<int>(2, 15)(mt);
		std::string S(N, '\0');
		for (auto &c : S) c = std::uniform_int_distribution<char>('a', 'z')(mt);
		auto A = Alisa(S, V);
		auto E = Eva(A);
		assert(S == E);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	test();

	std::string op; cin >> op;
	if (op == "Alisa") {
		std::string S; cin >> S;
		int L; cin >> L;
		std::vector<int> V(L);
		for (auto& v : V) cin >> v;
		auto A = Alisa(S, V);
		cout << A.size() << '\n';
		for (int i = 0; i < int(A.size()); i++) {
			cout << A[i] << " \n"[i+1==int(A.size())];
		}
	} else if (op == "Eva") {
		int L; cin >> L;
		std::vector<int> V(L);
		for (auto& v : V) cin >> v;
		auto E = Eva(V);
		cout << E << '\n';
	} else assert(false);

	return 0;
}
