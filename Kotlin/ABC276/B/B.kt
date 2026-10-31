private fun readLn() = readLine()!! // string line
private fun readInt() = readLn().toInt() // single int
private fun readStrings() = readLn().split(" ") // list of strings
private fun readInts() = readStrings().map { it.toInt() } // list of ints

private val OUTPUT_LINES = mutableListOf<String>()
private fun outputLn(s: String) { OUTPUT_LINES += s }

fun go() {
    val (N, M) = readInts()
    val adj = List(N, { mutableListOf<Int>() })
    for (e in 0 until M) {
        val (A, B) = readInts()
        adj[A-1] += B-1
        adj[B-1] += A-1
    }
    for (l in adj) {
        l.sort()
        //val len = l.size
        val inds = l.map { it + 1 }.joinToString(" ")
        outputLn("${l.size} ${inds}")
    }
}

fun main() {
    go()
    println(OUTPUT_LINES.joinToString("\n"))
}
