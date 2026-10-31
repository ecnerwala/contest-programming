private fun readLn() = readLine()!! // string line
private fun readInt() = readLn().toInt() // single int
private fun readStrings() = readLn().split(" ") // list of strings
private fun readInts() = readStrings().map { it.toInt() } // list of ints

private val OUTPUT_LINES = mutableListOf<String>()
private fun outputLn(s: String) { OUTPUT_LINES += s }

fun go() {
    var (n, k) = readInts()
    for (i in 0..k-1) {
        if (n % 10 == 0) {
            n /= 10
        } else {
            n--
        }
    }
    outputLn("${n}")
}

fun main() {
    go()
    println(OUTPUT_LINES.joinToString("\n"))
}
