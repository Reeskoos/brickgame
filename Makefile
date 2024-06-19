all: cmake_build

build_dir:
	mkdir -p build

cmake_configure: build_dir
	cd build && cmake ..

build/brick_game_console: cmake_configure
	cd build && cmake --build . --target brick_game_console

build/brick_game_desktop: cmake_configure
	cd build && cmake --build . --target brick_game_desktop

cmake_build: build/brick_game_console build/brick_game_desktop
	@cp build/brick_game_console .
	@cp build/brick_game_desktop .
	
coverage_configure: build_dir
	cd build && cmake -DCMAKE_BUILD_TYPE=Coverage ..

build/coverage: coverage_configure
	cd build && cmake --build . --target coverage

gcov_report: build/coverage

test: check_gtest cmake_build
	cd build && ./run_tests

test_val: test
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes -s ./build/run_tests

rebuild: clean all

check_gtest:
	@if [ ! -d "/usr/include/gtest" ] && [ ! -d "external/googletest" ]; then \
		echo "Google Test not found, cloning..."; \
		mkdir -p external; \
		cd external; \
		git clone https://github.com/google/googletest.git; \
	fi

install: cmake_build 
	sudo install -m 755 build/brick_game_console /usr/local/bin
	sudo install -m 755 build/brick_game_desktop /usr/local/bin

uninstall:
	sudo rm -f /usr/local/bin/brick_game_console
	sudo rm -f /usr/local/bin/brick_game_desktop

dvi: 
	doxygen Doxyfile
	xdg-open docs/html/index.html

dist: clean_all
	tar -czvf s21_brick_game.tar.gz *

clean_docs:
	rm -rf docs

clean: 
	rm -rf build
	rm -f brick_game_desktop
	rm -f brick_game_console
	
clean_all: uninstall clean clean_docs
