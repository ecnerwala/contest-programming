#include<bits/stdc++.h>

const int MAXN = 1 << 24;
uint8_t input[MAXN];
uint8_t prv[MAXN];

uint8_t output[MAXN];

std::pair<int, int> prv_suff[256];
int prv_score[257];
int cur_score[256];

int main() {
	using namespace std;

	if (fopen("input.bin", "rb")) {
		freopen("input.bin", "rb", stdin);
		freopen("output.bin", "wb", stdout);
	}

	int N = int(fread(input, sizeof input[0], MAXN, stdin));

	const int INF = N+10;

	const int HI = ~int(255);

	for (int i = 0; i < 256; i++) {
		prv_score[i] = INF;
		cur_score[i] = INF;
		prv_suff[i] = {INF, 255};
	}
	prv_score[256] = 0;

	std::pair<int, int> cur_pref(INF, -1);
	for (int i = 0; i < N; i++) {
		if (prv_suff[i & 255].first+2 < cur_score[i & 255]) {
			cur_score[i & 255] = prv_suff[i & 255].first+2;
			prv[i] = uint8_t(i - prv_suff[i & 255].second - 1);
		}
		if (cur_pref.first+2 < cur_score[i & 255]) {
			cur_score[i & 255] = cur_pref.first+2;
			prv[i] = uint8_t(i - cur_pref.second - 1);
		}

		if ((i & 255) == 0) {
			cur_pref = min(cur_pref, {prv_score[256], i});
		} else {
			cur_pref = min(cur_pref, {cur_score[(i & 255) - 1], i});
		}

		{
			uint8_t l = input[i];
			int k = i - l - 1;
			if (k >= 0) {
				int cnd = (k > (i & HI)) ? cur_score[k - (i & HI) - 1] : prv_score[k - ((i & HI) - 256)];
				cnd += (input[k] != l);
				if (cnd < cur_score[i & 255]) {
					cur_score[i & 255] = cnd;
					prv[i] = l;
				}
			}
		}

		{
			uint8_t l = input[i];
			int k = i + l + 1;
			if (k < N && k <= (i & HI) + 255) {
				int cnd = ((i & 255) == 0 ? prv_score[256] : cur_score[(i & 255) - 1]) + (input[k] != l);
				if (cnd < cur_score[k & 255]) {
					cur_score[k & 255] = cnd;
					prv[k] = l;
				}
			}
		}

		//cerr << i << ' ' << cur_score[i & 255] << ' ' << int(prv[i]) << '\n';

		if (i % 256 == 255) {
			prv_score[0] = prv_score[256];
			for (int j = 1; j <= 256; j++) {
				prv_score[j] = cur_score[j-1];
				cur_score[j-1] = INF;
			}
			int mi = *std::min_element(prv_score, prv_score + 257);
			for (int j = 0; j <= 256; j++) {
				prv_score[j] -= mi;
			}

			for (int j = 0; j < 256; j++) {
				int st = (i & HI) + j;
				assert(st <= i+1);
				if (st >= N) {
					continue;
				}
				uint8_t l = input[st];
				int en = st + l + 1;
				if (en <= i || en >= N) {
					continue;
				}
				int k = en - (i + 1);
				int cnd = prv_score[j] + (input[en] != l);
				if (cnd < cur_score[k]) {
					cur_score[k] = cnd;
					prv[en] = l;
				}
			}
			for (int j = 255; j >= 0; j--) {
				prv_suff[j] = {prv_score[j], (i & HI) + j};
				if (j < 255) {
					prv_suff[j] = min(prv_suff[j], prv_suff[j+1]);
				}
			}

			cur_pref = {INF, -1};
		}
	}

	//for (int i = 0; i < N; i++) { cerr << int(prv[i]) << '\n'; }

	int output_sz = 0;
	auto write_change = [&](int i, int v) {
		if (v == input[i]) {
			return;
		}
		assert(0 <= i && i < N);
		output[output_sz++] = uint8_t((i >> 0) & 255);
		output[output_sz++] = uint8_t((i >> 8) & 255);
		output[output_sz++] = uint8_t((i >> 16) & 255);
		assert(0 <= v && v <= 255);
		output[output_sz++] = uint8_t(v);
	};

	for (int cur = N-1; cur != -1; ) {
		int l = prv[cur];
		write_change(cur, l);
		cur --;
		cur -= l;
		write_change(cur, l);
		cur--;
	}

	fwrite(output, 1, output_sz, stdout);

	return 0;
}
