import java.util.*

private fun readLn() = readLine()!! // string line
private fun readInt() = readLn().toInt() // single int
private fun readStrings() = readLn().split(" ") // list of strings
private fun readInts() = readStrings().map { it.toInt() } // list of ints

private val OUTPUT_LINES = mutableListOf<String>()
private fun outputLn(s: String) { OUTPUT_LINES += s }

fun go() {
    val n = readInt()
    val visitors = (0..n-1).map{
        val (c, p) = readInts()
        c to (p to it)
    }.sortedBy{it.first}

    val k = readInt()
    val rs = readInts().mapIndexed{a, b -> b to a}.sortedBy{it.first}

    val q = PriorityQueue<Pair<Int, Int>>(compareBy({it.first}))
    var i = 0

    var ans = 0
    val v = mutableListOf<Pair<Int,Int>>()
    for ((r, j) in rs) {
        while (i < n && visitors[i].first <= r) {
            q.add(visitors[i].second)
            i++
        }
        val top = q.poll()
        if (top != null) {
            val (p, ind) = top
            ans += p
            v.add(ind to j)
        }
    }
    outputLn("${v.size} ${ans}")
    v.forEach {
        outputLn("${it.first+1} ${it.second+1}")
    }
}

fun main() {
    go()
    println(OUTPUT_LINES.joinToString("\n"))
}
