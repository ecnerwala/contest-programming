import java.util.*;
import java.io.*;

public class Main {
    public static void main(String[] args) throws Exception { john_lennox_vs_richard_dawkins(args); }

    public static void john_lennox_vs_richard_dawkins(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        PrintWriter pw = new PrintWriter(System.out);

        StringTokenizer st = new StringTokenizer(br.readLine());
        int N = Integer.parseInt(st.nextToken());
        int M = Integer.parseInt(st.nextToken());

        Pair[] trees = new Pair[N];
        for (int i = 0; i < N; i++) {
            st = new StringTokenizer(br.readLine());
            trees[i] = new Pair(
                Integer.parseInt(st.nextToken()),
                Integer.parseInt(st.nextToken())
            );
        }
        Pair[] poles = new Pair[M];
        for (int j = 0; j < M; j++) {
            st = new StringTokenizer(br.readLine());
            poles[j] = new Pair(
                Integer.parseInt(st.nextToken()),
                Integer.parseInt(st.nextToken())
            );
        }

        Arrays.sort(poles);

        double tot_len = 0;
        for (int z = -1; z <= 1; z += 2) {
            ArrayList<Pair> hull = new ArrayList<Pair>();
            for (Pair pole : poles) {
                while (hull.size() >= 2 && cross(hull.get(hull.size()-2), hull.get(hull.size()-1), pole) * z <= 0) {
                    hull.remove(hull.size()-1);
                }
                hull.add(pole);
            }
            for (int i = 0; i+1 < hull.size(); i++) {
                Pair a = hull.get(i);
                Pair b = hull.get(i+1);
                tot_len += Math.hypot(a.a - b.a, a.b - b.b);
            }
        }
        int target = (int)Math.ceil(tot_len);

        int[] dp = new int[target+1];
        for (int i = 1; i <= target; i++) dp[i] = (int)1e9;
        for (Pair p : trees) {
            int m = p.a, t = p.b;
            for (int i = target; i >= 0; i--) {
                int j = Math.min(i + m, target);
                dp[j] = Math.min(dp[j], dp[i] + t);
            }
        }
        System.out.println(dp[target]);
    }

    public static int cross(Pair a, Pair b) {
        return a.a * b.b - a.b * b.a;
    }
    public static int cross(Pair a, Pair b, Pair c) {
        return cross(a,b) + cross(b,c) + cross(c,a);
    }
}

class Pair implements Comparable<Pair> {
    public int a, b;
    public Pair(int a_, int b_) {
        a = a_;
        b = b_;
    }

    @Override
    public int compareTo(Pair p) {
        if (a != p.a) return a - p.a;
        else return b - p.b;
    }

}
