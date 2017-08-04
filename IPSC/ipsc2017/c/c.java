import java.math.BigDecimal;
import java.math.BigInteger;
import java.util.*;

public class c {

    static BigDecimal go(int N, long A, long B, int M, int tot) {
        if (M == 0) tot += 2;
        BigDecimal works = BigDecimal.ZERO;
        BigDecimal tval = BigDecimal.ZERO;
        for (long i = (M == 0) ? (N + 100000) : M; i >= N; i--) {
            BigDecimal d = BigDecimal.ONE.divide(new BigDecimal(i)).pow(tot);
            for (int j = 0; j < N; j++) {
                d = d.divide(new BigDecimal(i - j));
            }
            tval = tval.add(d);
        }
        for (long i = B; i >= A; i--) {
            BigDecimal d = BigDecimal.ONE.divide(new BigDecimal(i)).pow(tot);
            for (int j = 0; j < N; j++) {
                d = d.divide(new BigDecimal(i - j));
            }
            works = works.add(d);
        }
        return works.divide(tval);
    }
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int T = sc.nextInt();
        for (int i = 0; i < T; i++) {
            int N = sc.nextInt();
            long A = sc.nextLong();
            long B = sc.nextLong();
            int M = sc.nextInt();
            int tot = 0;
            for (int j = 0; j < N; j++) {
                tot += sc.nextInt();
            }
            BigDecimal result = go(N, A, B, M, tot);
            System.out.println(result);
        }
    }
}
