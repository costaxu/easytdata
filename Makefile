LEX=flex
YACC=bison

all: json test_ed  test

json.l.cpp:json.l
	    $(LEX) -P"json2tdata_" -o$@ $< 

json.y.cpp:json.y
	    $(YACC) -d -o $@ $< -p"json2tdata_"

json : json.l.cpp json.y.cpp easytdata.cpp
	g++  $(CFLAGS) -o$@ $^

test_ed: test_ed.cpp easytdata.cpp
	g++ $(CFLAGS) -o$@ $^

test: test.cpp easytdata.cpp
	g++ $(CFLAGS) -o$@ $^
clean:
	rm -f json json.l.cpp json.y.cpp


