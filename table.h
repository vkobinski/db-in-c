typedef enum {
  INT,
  FLOAT,
  STRING,
} CellType;

typedef struct {
  char* name;
  CellType** types;
  char** column_names;
} Table;
