CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc -I/usr/include/SFML
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = $(shell find src -name '*.cpp')
OBJS = $(patsubst src/%.cpp,build/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)
TARGET = build/game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf build

rebuild: clean all

-include $(DEPS)

.PHONY: all clean rebuild