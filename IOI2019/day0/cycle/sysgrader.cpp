#include "cycle.h"
#include <cstdio>
#include <cassert>
#include <string>

using namespace std;

static const int cnt_lim = 35;
static int n, p, cnt;

[[noreturn]] inline static void wrong_answer(string reason) {
	// BEGIN SECRET
	const string output_secret = "f3697e79-76f0-4a15-8dc8-212253e98c61";
	printf("%s\n", output_secret.c_str());
	printf("WA\n");
	// END SECRET
	printf("%s\n", reason.c_str());
	fclose(stdout);
	exit(0);
}

bool jump(int x) {
	if (x < 0 || x >= n)
		wrong_answer("Invalid argument.");
	cnt++;
	if (cnt > cnt_lim)
		wrong_answer("Too many queries.");

	p = (p + x) % n;
	return (n - p) % n <= n / 2;
}

int main() {
	// BEGIN SECRET
	char secret[1000];
	assert(1 == scanf("%s", secret));
	const string input_secret = "3f130aac-d629-40d9-b3ad-b75ea9aa8052";
	if (string(secret) != input_secret) {
		const string output_secret = "f3697e79-76f0-4a15-8dc8-212253e98c61";
		printf("%s\n", output_secret.c_str());
		printf("SV\n");
		fclose(stdout);
		return 0;
	}
	// END SECRET
	assert(1 == scanf("%d", &n));
	assert(1 == scanf("%d", &p));
	fclose(stdin);

	cnt = 0;
	escape(n);
	if (p != 0)
		wrong_answer("The exit was not found.");

	// BEGIN SECRET
	const string output_secret = "f3697e79-76f0-4a15-8dc8-212253e98c61";
	printf("%s\n", output_secret.c_str());
	// END SECRET
	printf("OK\n");
	fprintf(stderr, "%d\n", cnt);
	fclose(stdout);
	return 0;
}
