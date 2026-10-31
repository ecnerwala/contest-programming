/**
 * Solution attempt for NAIPC 2019
 * Rocket Ship/Hover Craft
 * @author godmar
 */
import java.util.*;
import java.util.function.*;
import static java.lang.Math.*;

public class RocketShip_godmar_bad2 {
    public static void main(String []av) {
        Scanner s = new Scanner(System.in);
        int x = s.nextInt();
        int y = abs(s.nextInt());       // w.l.o.g.
        P dest = new P(x, y);
        double v = s.nextDouble();      // speed, miles/s
        double w = s.nextDouble();      // angular speed rad/s, either direction

        // handle simple case
        if (y == 0 && x > 0) {
            System.out.println(x/v);    // just go to destination
            return;
        }

        /*
         * The optimal strategy has to start turning right away - there is no
         * advantage in waiting to turn unless we're already pointing at the destination.
         */

        // the turning radius of the circle we'd be moving if we turned & moved simultaneously
        double radius = v / w;

        /* Compute overall journey time if we turned for 't' seconds before moving. */
        Function<Double, Double> timeTaken = (t) -> {
            // if I started moving at time t, I'd be riding on this circle
            P center = new P(1, 0).rotate(w * t).leftNormal().scaleToLength(radius);
            Circle c = new Circle(center, radius);

            // I would ride it until I saw the destination along a tangent
            P [] tpoints = c.tangentPoints(dest);
            if (tpoints.length == 0)                // point is inside, keep turning
                return Double.POSITIVE_INFINITY;

            // If there's 2, pick the one that makes a left turn to the destination
            P q = tpoints[0];
            if (tpoints.length > 1 && q.crossproduct(dest) <= 0)    // !isCCW()
                q = tpoints[1];

            double arcAngle = q.sub(center).theta() - center.scale(-1).theta();
            double timeOnArc = arcAngle * radius / v;
            double leftToDestination = q.dist(dest) / v;
            return t + timeOnArc + leftToDestination;
        };
        // and search.
        System.out.println(timeTaken.apply(ternarySearch(timeTaken, 0, dest.theta()/w)));
    }

    //********************** Canned code follows 

    /**
     * Return x in [a, b] such that f(x) is minimal.
     * f() must have exactly one minimum in [a, b]
     */
    static double ternarySearch(Function<Double, Double> f, double left, double right) {
        while (true) {
            if ((right - left) < 10 * Math.max(1e-16, Math.ulp(right)))
                return f.apply(left) < f.apply(right) ? left : right;

            double leftThird = (2*left + right)/3;
            double rightThird = (left + 2*right)/3;

            if (f.apply(leftThird) < f.apply(rightThird))
                right = rightThird; // discard right third
            else
                left = leftThird;   // discard left third
        }
    }

    //------------ START of Geometry classes ------------
    static class P {
        final double x, y;
        P(double x, double y)    { this.x = x; this.y = y; }
        P sub(P that)            { return new P(x - that.x, y - that.y); }
        P add(P that)            { return new P(x + that.x, y + that.y); }
        double dot(P that)       { return x * that.x + y * that.y; }
        P scale(double s)        { return new P(x * s, y * s); }
        double length()          { return sqrt(x*x + y*y); }    
        double length2()         { return x * x + y * y; }
        P leftNormal()           { return new P(-y, x); }   // rotateCCW(90deg)
        P normalize()            { double n = length(); return n > 0 ? new P(x/n, y/n) : origin(); }
        P scaleToLength(double l) { return normalize().scale(l); }
        // use if sin, cos are known
        P rotateCCW(double sinT, double cosT) {
            return new P(x * cosT - y * sinT,
                         x * sinT + y * cosT); }
        P rotate(double theta)   { return rotateCCW(sin(theta), cos(theta)); }
        // angle to horizontal (1, 0); result is in [-pi, pi] rad or (-180-180) deg
        double theta()           { return atan2(y, x); }
        // angle between two vectors, result is in [0, pi] rad (0-180 deg)
        double angleTo(P a)      { return PI/2-asin(this.dot(a) / this.length() / a.length()); }
        boolean isOrigin()       { return x == 0 && y == 0; }
        public String toString() { return String.format("(%f,%f)", this.x, this.y); }
        static P origin()        { return new P(0, 0); }
        double det(P that)       { return this.x * that.y - this.y * that.x; }
        double crossproduct(P that) { return this.det(that); }

        double dist(P to)        { return sub(to).length(); }

        /**
         * Compute x for a * x + b = 0 and ||x|| = C
         * where 'this' is a.  
         * Care must be taken to handle the case where
         * either a.x or a.y is near zero.
         */
        P [] solveDotProductConstrainedByNorm(double b, double C) {
            P a = this;
            if (a.isOrigin())
                throw new Error("degenerate case");

            boolean transpose = abs(a.x) > abs(a.y);
            a = transpose ? new P(a.y, a.x) : a;

            Double [] x = solvequadratic(a.length2(), 2.0 * b * a.x, b * b - a.y * a.y * C * C);
            P [] p = new P[x.length];
            for (int i = 0; i < x.length; i++) {
                double x1 = x[i];
                double x2 = ((-b - a.x * x1) / a.y);
                p[i] = transpose ? new P(x2, x1) : new P(x1, x2);
            }
            return p;
        }
    }

    /* Solve a * x^2 + b * x + c == 0 
     * Returns 0, 1, or 2 solutions. If 2 solutions x1, x2, guarantees that x1 < x2
     */
    static Double [] solvequadratic(double a, double b, double c) {
        double D = b * b - 4 * a * c;
        if (D < -EPS)
            return new Double [] { };
        D = max(D, 0);
        if (D == 0)
            return new Double [] { -b / 2.0 / a };
        double d = sqrt(D);
        // Numerically more stable, see
        // https://en.wikipedia.org/wiki/Loss_of_significance#A_better_algorithm
        double x1, x2;
        if (signum(b) == 0) {
            x1 = d / 2.0 / a;
            x2 = -x1;
        } else {
            x1 = (-b - signum(b) * d) / (2 * a);
            x2 = c / (a * x1);
        }
        return new Double[] { Math.min(x1, x2), Math.max(x1, x2) };
    }

    static double EPS = 1e-6;

    static class Circle {
        P c;
        double R;
        Circle(P c, double R) { this.c = c; this.R = R; }
        @Override
        public String toString() { return String.format("{%s, %.03f}", c, R); }

        /* Returns the tangent lines that the point p makes with this circle, if any. */
        P [] tangentPoints(P p)
        {
            // Let c +/- r be the tangent points.  Then there's a 'd' such that
            // p + d - r = c 
            // Since d r = 0, we multiply by r and get
            // (p - c) r - ||r|| = 0  subject to ||r|| = R
            P [] r = p.sub(c).solveDotProductConstrainedByNorm(-R*R, R);
            P [] tangents = new P[r.length];
            for (int i = 0; i < tangents.length; i++)
                tangents[i] = c.add(r[i]);
            return tangents;
        }
    }
}
