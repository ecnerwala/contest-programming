$(TARGET): $(TARGET).cpp $(TARGET).h grader.cpp $(PCH_FILES)
	$(LINK.cpp) -isystem $(PCH_ROOT) $< grader.cpp $(LOADLIBES) $(LDLIBS) -o $@
