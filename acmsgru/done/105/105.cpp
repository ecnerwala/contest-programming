#include<bits/stdc++.h>
using namespace std;

int main() {
	unsigned int N; scanf("%u", &N);
	printf("%u\n", (unsigned int) (N - N/3 - (N % 3 >= 1)));

	return 0;
}
