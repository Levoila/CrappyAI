CXX = g++
CPPFLAGS = -fPIC -O3 -std=c++11
LDFLAGS = -shared
TARGET_LIB = libnes.so

SRCDIR = src
TARGETDIR = lib

SRCS = $(wildcard $(SRCDIR)/*.cpp)
INCLUDES = $(wildcard $(SRCDIR)$*.h)
OBJS = $(SRCS:%.cpp=%.o)

dir_guard = mkdir -p $(@D)

.PHONY: all
all: $(TARGETDIR)/$(TARGET_LIB)

$(TARGETDIR)/$(TARGET_LIB): $(OBJS)
	$(dir_guard)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

$(OBJS): $(SRCDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS)
	
.PHONY: remove
remove: clean
	rm -f $(TARGETDIR)/$(TARGET_LIB) 
