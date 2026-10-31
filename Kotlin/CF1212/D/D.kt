private fun readLn() = readLine()!! // string line
private fun readInt() = readLn().toInt() // single int
private fun readStrings() = readLn().split(" ") // list of strings
private fun readInts() = readStrings().map { it.toInt() } // list of ints
private fun readLongs() = readStrings().map { it.toLong() } // list of ints

private val OUTPUT_LINES = mutableListOf<String>()
private fun outputLn(s: String) { OUTPUT_LINES += s }

private fun factor23(a: Long): Pair<Int, Long> {
    var v = a
    var p2 = 0
    while (v % 2 == 0L) {
        v /= 2
        p2 ++
    }
    return p2 to a
}

fun go() {
    val n = readInt()
    val a = readLongs().map{factor23(it)}.sortedWith(compareBy({ it.first }, { -it.second })).map {it.second}
    outputLn(a.joinToString(" "))
}

fun main() {
    go()
    println(OUTPUT_LINES.joinToString("\n"))
}
