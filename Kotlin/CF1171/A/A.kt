private fun readLn() = readLine()!! // string line
private fun readInt() = readLn().toInt() // single int
private fun readStrings() = readLn().split(" ") // list of strings
private fun readInts() = readStrings().map { it.toInt() } // list of ints

private val OUTPUT_LINES = mutableListOf<String>()
private fun outputLn(s: String) { OUTPUT_LINES += s }

fun go() {
    val T = readInt()
    for (t in 0..T-1) {
        val n = readInt()
        outputLn("${n/2}")
    }
}

fun main() {
    go()
    println(OUTPUT_LINES.joinToString("\n"))
}
