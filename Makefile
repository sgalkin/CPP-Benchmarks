CXXFLAGS += \
	-Wall \
	-Werror \
	-O2 \
	-std=c++11 \

CXXFLAGS += \
	$(shell pkg-config --cflags benchmark)

LDFLAGS += \
	$(shell pkg-config --libs benchmark)

SRCS = \
	sorted_vector.cpp \

all: ${SRCS:.cpp=}
	for p in $^; do ./$$p --benchmark_out=$$p.txt --benchmark_out_format=csv; done

clean:
	@-rm ${SRCS:.cpp=}
