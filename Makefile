CXXFLAGS = -std=c++20 -Wall -Wextra -g

test: test.cpp html_parser.cpp parsing_utils.cpp
	g++ $(CXXFLAGS) test.cpp html_parser.cpp parsing_utils.cpp -o test

main: main.cpp html_parser.cpp parsing_utils.cpp
	g++ $(CXXFLAGS) main.cpp html_parser.cpp parsing_utils.cpp -o main

clean:
	rm -f main test