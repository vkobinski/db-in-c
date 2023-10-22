EXEC=db
CFLAGS=-pedantic -Wall -Wextra
FILES=main.c reading_structs.c

build:
	gcc $(CFLAGS) -o $(EXEC) $(FILES)

build-test:
	gcc $(CFLAGS) -o $(EXEC) $(FILES) -g

run: build
	./$(EXEC)

debug: build-test
	gdb $(EXEC)

test: build
	bundle exec rspec ./rspec/main_spec.rb
