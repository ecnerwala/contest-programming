#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class Hexspeak {

public:

	string decode(long long c) {
		vector<char> res;
		while(c) {
			int v = c % 16;
			c /= 16;
			if(v >= 10) res.push_back('A' + v - 10);
			else if(v == 0) res.push_back('O');
			else if(v == 1) res.push_back('I');
			else return "Error!";
		}
		return string(res.rbegin(), res.rend());
	}
};

// vim:ft=cpp
