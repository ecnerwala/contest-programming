private fun readLn() = readLine()!! // string line
private fun readInt() = readLn().toInt() // single int
private fun readStrings() = readLn().split(" ") // list of strings
private fun readInts() = readStrings().map { it.toInt() } // list of ints

private val OUTPUT_LINES = mutableListOf<String>()
private fun outputLn(s: String) { OUTPUT_LINES += s }

fun go() {
    val (n, k) = readInts()
    val a = readInts().sorted()
    val v = if (k == 0) 1 else a[k-1]
    val ans = if (k == n || v < a[k]) v else -1
    outputLn("${ans}")
}

fun main() {
    go()
    println(OUTPUT_LINES.joinToString("\n"))
}
