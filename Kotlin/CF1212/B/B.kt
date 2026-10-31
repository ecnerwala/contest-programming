private fun readLn() = readLine()!! // string line
private fun readInt() = readLn().toInt() // single int
private fun readStrings() = readLn().split(" ") // list of strings
private fun readInts() = readStrings().map { it.toInt() } // list of ints

private val OUTPUT_LINES = mutableListOf<String>()
private fun outputLn(s: String) { OUTPUT_LINES += s }

fun go() {
    val n = readInt()
    val s = readLn().toCharArray()
    val cnt = mutableMapOf<Pair<Char, Char>, Int>()
    for (i in 0..n-2) {
        cnt[s[i] to s[i+1]] = (cnt[s[i] to s[i+1]] ?: 0) + 1
    }
    val ans = cnt.maxBy { it.value }!!.key
    outputLn("${ans.first}${ans.second}")
}

fun main() {
    go()
    println(OUTPUT_LINES.joinToString("\n"))
}
