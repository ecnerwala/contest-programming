private fun readLn() = readLine()!! // string line
private fun readInt() = readLn().toInt() // single int
private fun readStrings() = readLn().split(" ") // list of strings
private fun readInts() = readStrings().map { it.toInt() } // list of ints

private val OUTPUT_LINES = mutableListOf<String>()
private fun outputLn(s: String) { OUTPUT_LINES += s }


fun modInv(a: Int, m: Int): Int = if (a == 1) 1 else m - (modInv(m % a, a).toLong() * m.toLong() / a).toInt()

const val MOD = 998244353;
data class ModNum(val v: Int)
operator inline fun ModNum.unaryPlus() = this
operator inline fun ModNum.unaryMinus() = ModNum(if (v != 0) MOD - v else 0)
operator inline fun ModNum.plus(o: ModNum): ModNum {
    val r = v - (MOD - o.v)
    return ModNum(if (r < 0) r + MOD else r)
}
operator inline fun ModNum.minus(o: ModNum): ModNum {
    val r = v - o.v
    return ModNum(if (r < 0) r + MOD else r)
}
operator inline fun ModNum.times(o: ModNum): ModNum {
    return ModNum((v.toLong() * o.v.toLong() % MOD).toInt())
}
inline fun ModNum.recip(): ModNum {
    return ModNum(modInv(v, MOD))
}
operator inline fun ModNum.div(o: ModNum): ModNum {
    return times(o.recip())
}
inline fun pow(a_: ModNum, b_: Long): ModNum {
    var a = a_
    var b = b_
    var r = ModNum(1)
    while (b != 0L) {
        if (b and 1L == 1L) r *= a
        a *= a
        b = b shr 1
    }
    return r
}
inline fun pow(a_: ModNum, b_: Int): ModNum {
    var a = a_
    var b = b_
    var r = ModNum(1)
    while (b != 0) {
        if (b and 1 == 1) r *= a
        a *= a
        b = b shr 1
    }
    return r
}

val MOD_ROOT = ModNum(3)
const val S = 1 shl 13
val W = pow(MOD_ROOT, (MOD - 1) / S)

fun go() {
    val (N, Q) = readInts()
    val powW = Array<ModNum>(S) { ModNum(0) }
    var pref = ModNum(1)
    for (i in 0..S-1) {
        powW[i] = pref - ModNum(1)
        pref = pref * W
    }

    val cost: IntArray = readInts().toIntArray()
    val cap: IntArray = IntArray(N){0}

    for (q in 0..Q-1) {
        val query = readInts()
        val op = query[0]
        if (op == 1 || op == 2) {
            val i = query[1]-1
            val c = query[2] * (if (op == 2) -1 else 1)

            cap[i] += c

        } else if (op == 3) {
            val l = query[1]-1
            val r = query[2]
            val k = query[3]
        } else assert(false);
    }
}

fun main() {
    go()
    println(OUTPUT_LINES.joinToString("\n"))
}
