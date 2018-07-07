TARGETS := main
CXXFLAGS += -g -Wall -std=c++11 -lpthread
CXX=clang++
all: $(TARGETS)

clean:
	@rm -rf ./$(TARGETS)

run: $(TARGETS)
	@./$(TARGETS)