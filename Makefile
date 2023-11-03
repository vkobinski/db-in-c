EXEC=db
CFLAGS=-pedantic -Wall -Wextra
FILES=./src/main.c ./src/reading_structs.c ./src/metacommand.c ./src/record.c ./src/string_utils.c

buil:
	gcc $(CFLAGS) -o $(EXEC) $(FILES)

build-test:
	gcc $(CFLAGS) -o $(EXEC) $(FILES) -g

run: build
	./$(EXEC) dbfile

debug: build-test
	gdb $(EXEC) dbfile

rspec: build
	bundle exec rspec ./rspec/main_spec.rb
