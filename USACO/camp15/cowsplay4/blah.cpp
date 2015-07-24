#include<bits/stdc++.h>

using namespace std;
const string v[10] = {
"000022120",
"002210220",
"122220000",
"011210000",
"112011201",
"122220020",
"112211200",
"120002210",
"122000010",
"012112210"
};
int process(string s) {
	int res = s[0] - '0';
	for(int b = 2, i = 1; i < int(s.size()); i++, b *= 3) {
		res += b * (s[i] - '0');
	}
	return res;
}
int main() {
	for(int i = 0; i < 10; i++) {
		cout << process(v[i]) << '\n';
	}
}
