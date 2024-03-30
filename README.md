# SQLite Clone In C

A study of how a database works by implementing one.

The database now has the following functions:
 - Select all Rows
 - Insert a Row
 - Create a table
 - Get table information

Right now the implementation supports one row per file. The table and its information is persisted in a file that is passed to the program as an argument.

## File Architecture

The persistence of the database is done by writing the table information in a file. The file has a header in the start of the file, then the header is structured as follows:
```
header_size: size_t
col_count: uint8_t
table_name_size: size_t
table_name: char[table_name_size]
col_names: char[col_count][MAX_COL_NAME_SIZE]
# For each Column
...
col_type: uint8_t
col_name_size: size_t
col_name: char[col_name_size]
...
```

## Queries
- `CREATE TABLE <table_name> (<column_name>:<column_type>, ...)`: Creates a table with the given name and column. The column type can be `REAL`, `TEXT` and `INTEGER`. The `ID` column is automatically created in the table.
- `INSERT (<value>, ...)`: Inserts a row with the given values. The program checks the number of values and the types of the columns to insert the row.
- `SELECT`: Selects all rows from the table.

## Metacommands
- `.exit`: Exits and saves the program.
- `.info`: Shows the table information.
- `.help`: Shows the help message.

## How to run
To run the program, you need to have the `gcc` compiler and `make` installed. Then, you can run the following commands:

```bash
make run
```

## How to run integration tests
To run the tests, you need to install:
- Ruby
- bundle

Then, you can run the following commands:

```bash
make rspec
```