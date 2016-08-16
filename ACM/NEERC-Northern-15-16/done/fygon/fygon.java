import java.lang.*;
import java.util.*;
import java.math.*;
import java.io.*;

public class fygon {

	static int L;
	static char[][] lines = new char[30][];
	static int[] ind = new int[30];
	static int[] val = new int[30];

	static int sim(int line, int indent) {
		if(line == L) return 0;

		if(indent < ind[line]) return sim(line + 1, indent);
		if(indent > ind[line]) return 0;

		if(lines[line][0] == 'l') {
			return 1 + sim(line + 1, indent);
		} else {

			int var = lines[line][4] - 'a';

			int max;
			char c = lines[line][15];
			if('0' <= c && c <= '9') {
				max = c - '0';
			} else {
				max = val[c - 'a'];
			}

			int res = 0;

			for(val[var] = 0; val[var] < max; val[var] ++) {
				res += sim(line + 1, indent + 1);
			}

			res += sim(line + 1, indent);

			return res;
		}
	}

	static long det(long[][] x) {
		assert(x.length == 7);

		BigInteger res = BigInteger.ZERO;
		for(int a = 0; a < 7; a++) {
			for(int b = 0; b < 7; b++) {
				if(b == a) continue;
				for(int c = 0; c < 7; c++) {
					if(c == a || c == b) continue;
					for(int d = 0; d < 7; d++) {
						if(d == a || d == b || d == c) continue;
						for(int e = 0; e < 7; e++) {
							if(e == a || e == b || e == c || e == d) continue;
							for(int f = 0; f < 7; f++) {
								if(f == a || f == b || f == c || f == d || f == e) continue;

								int g = 0 + 1 + 2 + 3 + 4 + 5 + 6 - a - b - c - d - e - f;

								int sign = 1;

								if(b < a) sign *= -1;

								if(c < a) sign *= -1;
								if(c < b) sign *= -1;

								if(d < a) sign *= -1;
								if(d < b) sign *= -1;
								if(d < c) sign *= -1;

								if(e < a) sign *= -1;
								if(e < b) sign *= -1;
								if(e < c) sign *= -1;
								if(e < d) sign *= -1;

								if(f < a) sign *= -1;
								if(f < b) sign *= -1;
								if(f < c) sign *= -1;
								if(f < d) sign *= -1;
								if(f < e) sign *= -1;

								if(g < a) sign *= -1;
								if(g < b) sign *= -1;
								if(g < c) sign *= -1;
								if(g < d) sign *= -1;
								if(g < e) sign *= -1;
								if(g < f) sign *= -1;

								BigInteger val = BigInteger.valueOf(
										sign *
										x[0][a] *
										x[1][b] *
										x[2][c] *
										x[3][d] *
										x[4][e] *
										x[5][f] *
										x[6][g]);

								assert(val.signum() != -sign);

								res = res.add(val);

							}
						}
					}
				}
			}
		}

		return res.longValue();
	}

	public static void main(String[] args) throws IOException {
		BufferedReader input = new BufferedReader(new FileReader("fygon.in"));
		PrintWriter output = new PrintWriter(new FileWriter("fygon.out"));
		while(true) {
			String s = input.readLine();
			if(s == null) break;
			ind[L] = 0;
			while(s.charAt(ind[L]) == ' ') ind[L] ++;
			lines[L] = s.substring(ind[L]).toCharArray();
			assert(ind[L] % 4 == 0);
			ind[L] /= 4;
			L++;
		}
		long[][] x = new long[7][8];
		for(int n = 0; n < 7; n++) {
			val['n' - 'a'] = n;
			x[n][0] = 1;
			x[n][1] = x[n][0] * n;
			x[n][2] = x[n][1] * n;
			x[n][3] = x[n][2] * n;
			x[n][4] = x[n][3] * n;
			x[n][5] = x[n][4] * n;
			x[n][6] = x[n][5] * n;

			x[n][7] = sim(0, 0);
		}

		long div = det(x);

		long[] coeff = new long[7];
		for(int i = 0; i < 7; i++) {
			for(int j = 0; j < 7; j++) {
				long tmp = x[j][i];
				x[j][i] = x[j][7];
				x[j][7] = tmp;
			}
			coeff[i] = det(x);
			for(int j = 0; j < 7; j++) {
				long tmp = x[j][i];
				x[j][i] = x[j][7];
				x[j][7] = tmp;
			}
		}

		output.printf("1 / %d * ((((((n * %d + %d) * n + %d) * n + %d) * n + %d) * n + %d) * n + %d)\n",
				div, coeff[6], coeff[5], coeff[4], coeff[3], coeff[2], coeff[1], coeff[0]);

		output.close();
	}
}
