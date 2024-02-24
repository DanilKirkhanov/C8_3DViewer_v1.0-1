CC 			= gcc
CFLAGS 		= -std=c11 -Wall -Werror -Wextra
TEST_LIBS 	= -lm $(shell pkg-config --cflags --libs check)
SOURCES 	= $(wildcard *.c)
OBJECTS 	= $(patsubst %.c, %.o, $(SOURCES))
PROJECT		= s21_3dviewer
STATIC_LIB	= 3D_Viewer.a

all: uninstall install dvi dist


install: delete_temp_build recreate_build_dir
	@mkdir .temp_build
	@cd .temp_build/ && qmake ../3d_viewer/3d_viewer.pro && make && mv 3d_viewer.app ../build/3d_viewer.app
	rm -rf .temp_build

delete_temp_build:
	rm -rf .temp_build

recreate_build_dir:
	@rm -rf build
	@mkdir build

dist: install
	@rm -rf dist.tar.gz
	@tar -cvzf dist.tar.gz build/3d_viewer.app

uninstall: 
	@rm -rf build

gcov_flag:
	$(eval CFLAGS += --coverage)
	
clean: dvi_clean
	rm -rf Documentation test gcov
	rm -rf *.o *.a *.gcda *.gcno *.gch *.pdf *.tar rep.info test.info test.dSYM coverage.info test

gcov_report: clean gcov_flag test
	mkdir -p gcov
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory gcov
	rm -rf *.gcno *.gcda *.gcov
	open gcov/index.html

$(STATIC_LIB): clean $(OBJECTS)
	ar -rcs $(STATIC_LIB) $(OBJECTS)
	ranlib $(STATIC_LIB)
	rm -f *.o


test: clean $(STATIC_LIB)
	$(CC) $(CFLAGS) Tests/*.c -o test $(STATIC_LIB) $(TEST_LIBS) 
	./test

dvi: dvi_clean
	makeinfo manual.texi --html
	open ./manual/index.html

dvi_clean:
	rm -rf manual