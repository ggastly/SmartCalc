COMPILE = g++
COMPILEFLAGS = -std=c++17 -Wall -Wextra -Werror
LIBS = -lgtest -lgtest_main -lpthread
SRC_DIR = smart_calc
BUILD_DIR = build
ARCHIVE_DIR := archive_calculator
ARCHIVE_NAME := archive_calculator.tgz

all: install

install:
	if [ -d "$(BUILD_DIR)" ] && [ -f "$(BUILD_DIR)/SmartCalc.app/Contents/MacOS/SmartCalc" ]; then \
		echo "The SmartCalc application is already installed in the ${PWD}/$(BUILD_DIR) folder"; \
	else \
		mkdir -p $(BUILD_DIR); \
		cd $(SRC_DIR) && qmake && make -j6 && make clean && rm Makefile; \
		cd ..; \
		mv $(SRC_DIR)/SmartCalc.app $(BUILD_DIR); \
		echo "The SmartCalc application has been successfully installed and is in ${PWD}/$(BUILD_DIR)"; \
	fi

uninstall:
	if [ -d "$(BUILD_DIR)" ]; then \
		rm -r $(BUILD_DIR); \
		echo "The application was successfully deleted"; \
	else \
		echo "The application is not installed"; \
	fi

clean:
	rm -rf *.a && rm -rf *.o  && rm -rf *.dSYM && rm -rf *.out
	rm -rf *.info && rm -rf test && rm -rf Dist_smart_calc && rm -rf *tgz && rm -rf build && rm -rf .qmake.stash

dvi:
	open README.md

dist:
	if [ -d "$(BUILD_DIR)" ] && [ -f "$(BUILD_DIR)/SmartCalc.app/Contents/MacOS/SmartCalc" ]; then \
		echo "Begin packaging process"; \
		mkdir -p $(ARCHIVE_DIR); \
		mv $(BUILD_DIR)/SmartCalc.app $(ARCHIVE_DIR)/; \
		tar cvzf $(ARCHIVE_NAME) $(ARCHIVE_DIR)/; \
		rm -rf $(ARCHIVE_DIR)/; \
		echo "Build completed"; \
	else \
		make install; \
		make dist; \
	fi

tests: clean
	$(COMPILE) $(COMPILEFLAGS) $(SRC_DIR)/tests/tests.cc -g -o test $(LIBS)
	./test

leaks: clean
	$(COMPILE) $(COMPILEFLAGS) $(SRC_DIR)/tests/tests.cc -lgtest -o leaks
	CK_FORK=no leaks --atExit -- ./leaks
	rm ./leaks

leaks_app:
	CK_FORK=no leaks --atExit -- ./$(BUILD_DIR)/SmartCalc.app/Contents/MacOS/SmartCalc

open:
	if [ -d "$(BUILD_DIR)" ] && [ -f "$(BUILD_DIR)/SmartCalc.app/Contents/MacOS/SmartCalc" ]; then \
		./$(BUILD_DIR)/SmartCalc.app/Contents/MacOS/SmartCalc; \
	else \
		echo "application not found"; \
	fi

check:
	cp ../materials/linters/.clang-format ./
	clang-format -n smart_calc/*/*.cc smart_calc/*/*.h

.PHONY: all clean install uninstall dist tests leak_app open check