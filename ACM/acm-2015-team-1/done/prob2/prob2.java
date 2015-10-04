import java.io.*;
import java.util.*;
import java.math.*;

public class prob2 {
	BufferedReader br;
	StringTokenizer st = null;
	final int K;
	final int S;
	final int M;

	public prob2() {
		br = new BufferedReader(new InputStreamReader(System.in));
		K = ni();
		S = ni();
		M = ni();
	}

	BigInteger makeBi(int x) {
		return BigInteger.valueOf(x);
	}

	String nt() {
		try {
			if (st == null || !st.hasMoreTokens()) {
				st = new StringTokenizer(br.readLine());
			}
		} catch(Exception e) {
			return "";
		}
		return st.nextToken();
	}
		
	int ni() {
		return Integer.parseInt(nt());
	}

	BigInteger[] cs = new BigInteger[10008];
	// cs[i] = choose(i + k - 1, k - 1)

	BigInteger choose(int a, int b) {
		BigInteger x = makeBi(1);
		for (int i = 1; i <= b; i++) {
			x = x.multiply(makeBi(a + 1 - i));
			x = x.divide(makeBi(i));
		}
		return x;
	}
	void prec() {
		for (int i = 0; i < 10004; i++) {
			cs[i] = choose(i + K, K);
		}
	}

	int[][] d = new int[28][38];

	void run() {
		BigInteger gs = makeBi(0);
		prec();
		for (int i = 0; i < M; ++i) {
			for(int j = 0; j < K; ++j) {
				d[i][j] = ni();
				// System.out.println("Read " + a);
			}
		}
		for(int i = 0; i < (1 << M); ++i) {
			int[] b = new int[K];
			for (int j = 0; j < M; ++j) {
				if ((i & (1<<j)) != 0) {
					for (int k = 0; k < K; k++) {
						b[k] = Math.max(b[k], d[j][k]);
					}
				}
			}
			int s = S;
			for (int j = 0; j < K; ++j) { s -= b[j]; }

			if (s < 0) continue;

			if (Integer.bitCount(i) % 2 == 1) {
				gs = gs.subtract(cs[s]);
			} else {
				gs = gs.add(cs[s]);
			}
		}
		
		int subtr = S * K + 1;
		gs = gs.subtract(makeBi(subtr));
		System.out.println(gs);
	}

	public static void main(String[] args) {
		new prob2().run();
	}
}
