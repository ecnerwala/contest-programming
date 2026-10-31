%.jar: %.kt
	kotlinc $< -include-runtime -d $@
