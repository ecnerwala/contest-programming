/**
 * Solution attempt for NAIPC 2019
 * Rocket Ship/Hover Craft
 * @author godmar
 */
import java.util.*;
import java.util.function.*;
import static java.lang.Math.*;

public class RocketShip_godmar_float {
    public static void main(String []av) {
        Scanner s = new Scanner(System.in);
        int x = s.nextInt();
        int y = abs(s.nextInt());       // w.l.o.g.
        P dest = new P(x, y);
        float v = s.nextFloat();      // speed, miles/s
        float w = s.nextFloat();      // angular speed rad/s, either direction

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
        float radius = v / w;

        /* Compute overall journey time if we turned for 't' seconds before moving. */
        Function<Float, Float> timeTaken = (t) -> {
            // if I started moving at time t, I'd be riding on this circle
            P center = new P(1, 0).rotate(w * t).leftNormal().scaleToLength(radius);
            Circle c = new Circle(center, radius);

            // I would ride it until I saw the destination along a tangent
            P [] tpoints = c.tangentPoints(dest);
            if (tpoints.length == 0)                // point is inside, keep turning
                return Float.POSITIVE_INFINITY;

            // If there's 2, pick the one that makes a left turn to the destination
            P q = tpoints[0];
            if (tpoints.length > 1 && q.crossproduct(dest) <= 0)    // !isCCW()
                q = tpoints[1];

            float arcAngle = q.sub(center).angleTo(center.scale(-1));
            float timeOnArc = arcAngle * radius / v;
            float leftToDestination = q.dist(dest) / v;
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
    static float ternarySearch(Function<Float, Float> f, float left, float right) {
        while (true) {
            if ((right - left) < 10 * Math.max(1e-16, Math.ulp(right)))
                return (left + right)/2.0f;

            float leftThird = (2f*left + right)/3f;
            float rightThird = (left + 2f*right)/3f;

            if (f.apply(leftThird) < f.apply(rightThird))
                right = rightThird; // discard right third
            else
                left = leftThird;   // discard left third
        }
    }

    //------------ START of Geometry classes ------------
    static class P {
        final float x, y;
        P(float x, float y)    { this.x = x; this.y = y; }
        P sub(P that)            { return new P(x - that.x, y - that.y); }
        P add(P that)            { return new P(x + that.x, y + that.y); }
        float dot(P that)       { return x * that.x + y * that.y; }
        P scale(float s)        { return new P(x * s, y * s); }
        float length()          { return (float) sqrt(x*x + y*y); }    
        float length2()         { return x * x + y * y; }
        P leftNormal()           { return new P(-y, x); }   // rotateCCW(90deg)
        P normalize()            { float n = length(); return n > 0 ? new P(x/n, y/n) : origin(); }
        P scaleToLength(float l) { return normalize().scale(l); }
        // use if sin, cos are known
        P rotateCCW(float sinT, float cosT) {
            return new P(x * cosT - y * sinT,
                         x * sinT + y * cosT); }
        P rotate(float theta)   { return rotateCCW((float)sin(theta), (float)cos(theta)); }
        // angle to horizontal (1, 0); result is in [-pi, pi] rad or (-180-180) deg
        float theta()           { return (float) atan2(y, x); }
        // angle between two vectors, result is in [0, pi] rad (0-180 deg)
        float angleTo(P a)      { return (float) acos(this.dot(a) / this.length() / a.length()); }
        boolean isOrigin()       { return x == 0 && y == 0; }
        public String toString() { return String.format("(%f,%f)", this.x, this.y); }
        static P origin()        { return new P(0, 0); }
        float det(P that)       { return this.x * that.y - this.y * that.x; }
        float crossproduct(P that) { return this.det(that); }

        float dist(P to)        { return sub(to).length(); }

        /**
         * Compute x for a * x + b = 0 and ||x|| = C
         * where 'this' is a.  
         * Care must be taken to handle the case where
         * either a.x or a.y is near zero.
         */
        P [] solveDotProductConstrainedByNorm(float b, float C) {
            P a = this;
            if (a.isOrigin())
                throw new Error("degenerate case");

            boolean transpose = abs(a.x) > abs(a.y);
            a = transpose ? new P(a.y, a.x) : a;

            Float [] x = solvequadratic(a.length2(), 2.0f * b * a.x, b * b - a.y * a.y * C * C);
            P [] p = new P[x.length];
            for (int i = 0; i < x.length; i++) {
                float x1 = x[i];
                float x2 = ((-b - a.x * x1) / a.y);
                p[i] = transpose ? new P(x2, x1) : new P(x1, x2);
            }
            return p;
        }
    }

    /* Solve a * x^2 + b * x + c == 0 
     * Returns 0, 1, or 2 solutions. If 2 solutions x1, x2, guarantees that x1 < x2
     */
    static Float [] solvequadratic(float a, float b, float c) {
        float D = b * b - 4 * a * c;
        if (D < -EPS)
            return new Float [] { };
        D = max(D, 0);
        if (D == 0)
            return new Float [] { -b / 2.0f / a };
        float d = (float) sqrt(D);
        // Numerically more stable, see
        // https://en.wikipedia.org/wiki/Loss_of_significance#A_better_algorithm
        float x1, x2;
        if (signum(b) == 0) {
            x1 = d / 2.0f / a;
            x2 = -x1;
        } else {
            x1 = (-b - signum(b) * d) / (2 * a);
            x2 = c / (a * x1);
        }
        return new Float[] { Math.min(x1, x2), Math.max(x1, x2) };
    }

    static float EPS = 1e-6f;

    static class Circle {
        P c;
        float R;
        Circle(P c, float R) { this.c = c; this.R = R; }
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
