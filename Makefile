EXEC=db
CFLAGS=-pedantic -Wall -Wextra
FILES=./src/main.c ./src/reading_structs.c ./src/metacommand.c ./src/record.c ./src/string_utils.c

buil:
	gcc $(CFLAGS) -o $(EXEC) $(FILES)

build-test:
	gcc $(CFLAGS) -o $(EXEC) $(FILES) -g

run: buil
	./$(EXEC) dbfile

debug: build-test
	gdb $(EXEC) dbfile

rspec: buil
	bundle exec rspec ./rspec/main_spec.rb
