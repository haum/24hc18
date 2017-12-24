CXXFLAGS+=-std=c++14 -Wall -gdwarf -Og -fkeep-inline-functions -Wextra -pipe -funsigned-char -fstrict-aliasing -Wchar-subscripts -Wundef -Wshadow -Wcast-align -Wwrite-strings -Wunused -Wuninitialized -Wpointer-arith -Wredundant-decls -Wformat -Wformat-security -Winit-self -Wmissing-include-dirs -Wmissing-declarations -MMD -MP

CPPFILES+=$(wildcard *.cpp */*.cpp)
OBJS=$(patsubst %.cpp,build/%.o,${CPPFILES})

all: marabunta_engine

marabunta_engine:${OBJS}
	${CXX} ${CXXFLAGS} -o $@ ${OBJS} ${LDFLAGS}

format:${CPPFILES} $(wildcard *.h */*.h)
	clang-format -style=file -i $^

build/%.o:%.cpp
	@mkdir -p build/$(dir $<)
	$(CXX) ${CXXFLAGS} -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	rm -rf build
	rm -f marabunta_engine
