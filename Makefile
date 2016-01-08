CC := g++ -std=c++14
CFLAGS := -Wall -I.

SRCDIR = src
BUILDDIR = build
TESTDIR = tests

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
SRC_OBJS := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TESTS   := $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJS := $(TESTS:$(TESTDIR)/%.cpp=$(BUILDDIR)/%.o)

test: $(SRC_OBJS) $(TEST_OBJS)
	$(CC) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $^

$(BUILDDIR)/%.o: $(TESTDIR)/%.cpp 
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -r $(BUILDDIR) test
