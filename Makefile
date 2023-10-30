EXEC=db
CFLAGS=-pedantic -Wall -Wextra
FILES=main.c ./src/reading_structs.c ./src/metacommand.c ./src/record.c ./src/statements.c

build:
	gcc $(CFLAGS) -o $(EXEC) $(FILES)

build-test:
	gcc $(CFLAGS) -o $(EXEC) $(FILES) -g

run: build
	./$(EXEC) dbfile

debug: build-test
	gdb $(EXEC) dbfile

test: build
	bundle exec rspec ./rspec/main_spec.rb
