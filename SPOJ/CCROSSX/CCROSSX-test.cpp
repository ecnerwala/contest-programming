#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MAXV = 1e9;

string in = "data.in";
string out = "data.out";
string res = "data.res";

void generate(int N, ll D) {
	ofstream infile (in);

	infile << 1 << '\n';

	infile << N << ' ' << D << '\n';

	for(int i = 0; i < N; i++) {
		infile << (ll(rand()) << 31 + rand()) % (MAXV + 1) << '\n';
	}
}


int main() {
}
