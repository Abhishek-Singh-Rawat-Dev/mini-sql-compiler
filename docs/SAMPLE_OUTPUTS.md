# Sample Outputs
## Mini SQL Compiler - Console Output Examples

---

## 1. Successful Query Validation

### Input
```sql
SELECT name, age FROM employees WHERE age > 25;
```

### Console Output
```
╔══════════════════════════════════════════════════════════════╗
║          MINI SQL COMPILER FOR QUERY VALIDATION              ║
║                   Compiler Design Project                    ║
╠══════════════════════════════════════════════════════════════╣
║  Team Members:                                               ║
║    Member 1: Lexical Analyzer                                ║
║    Member 2: Syntax Analyzer                                 ║
║    Member 3: Semantic Analyzer                               ║
║    Member 4: Error Handler & Testing                         ║
╚══════════════════════════════════════════════════════════════╝

══════════════════════════════════════════
Starting compilation of query...
══════════════════════════════════════════

========================================
   PHASE 1: LEXICAL ANALYSIS
========================================
Input Query: SELECT name, age FROM employees WHERE age > 25;

Lexical Analysis: SUCCESS
Total Tokens Generated: 12

--- Token Stream ---
+-----------------------+------------------+------+-----+
| Token Type            | Value            | Line | Col |
+-----------------------+------------------+------+-----+
| KEYWORD_SELECT        | SELECT           |    1 |   1 |
| IDENTIFIER            | name             |    1 |   8 |
| OP_COMMA              | ,                |    1 |  12 |
| IDENTIFIER            | age              |    1 |  14 |
| KEYWORD_FROM          | FROM             |    1 |  18 |
| IDENTIFIER            | employees        |    1 |  23 |
| KEYWORD_WHERE         | WHERE            |    1 |  33 |
| IDENTIFIER            | age              |    1 |  39 |
| OP_GREATER_THAN       | >                |    1 |  43 |
| NUMBER                | 25               |    1 |  45 |
| OP_SEMICOLON          | ;                |    1 |  47 |
| END_OF_INPUT          |                  |    1 |  48 |
+-----------------------+------------------+------+-----+

========================================
   PHASE 2: SYNTAX ANALYSIS
========================================
Syntax Analysis: SUCCESS
Parse Tree constructed successfully.

--- Parse Tree (Intermediate Representation) ---
|-- QUERY
  |-- SELECT_CLAUSE: "SELECT"
    |-- COLUMN_LIST
      |-- COLUMN: "name"
      |-- COLUMN: "age"
  |-- FROM_CLAUSE: "FROM"
    |-- TABLE_NAME: "employees"
  |-- WHERE_CLAUSE: "WHERE"
    |-- CONDITION
      |-- COLUMN: "age"
      |-- OPERATOR: ">"
      |-- VALUE: "25"

--- Available Schema for Validation ---
+------------------+------------------+----------+
| Table            | Column           | Type     |
+------------------+------------------+----------+
| employees        | id               | INT      |
|                  | name             | VARCHAR  |
|                  | age              | INT      |
|                  | salary           | FLOAT    |
|                  | department       | VARCHAR  |
+------------------+------------------+----------+

========================================
   PHASE 3: SEMANTIC ANALYSIS
========================================
Table 'employees' validated.
Column 'name' validated in table 'employees'.
Column 'age' validated in table 'employees'.
Column 'age' validated in table 'employees'.
Condition validated: age > 25
Semantic Analysis: SUCCESS
All identifiers resolved correctly.

========================================
   COMPILATION SUMMARY
========================================

+-------------------+----------------+
| Phase             | Status         |
+-------------------+----------------+
| Lexical Analysis  | PASSED         |
| Syntax Analysis   | PASSED         |
| Semantic Analysis | PASSED         |
+-------------------+----------------+

╔═══════════════════════════════════════╗
║   QUERY VALIDATION: SUCCESSFUL        ║
║   The SQL query is valid!             ║
╚═══════════════════════════════════════╝
```

---

## 2. Simple SELECT * Query

### Input
```sql
SELECT * FROM users;
```

### Console Output (Abbreviated)
```
========================================
   PHASE 1: LEXICAL ANALYSIS
========================================
Input Query: SELECT * FROM users;

Lexical Analysis: SUCCESS
Total Tokens Generated: 5

--- Token Stream ---
| KEYWORD_SELECT        | SELECT           |    1 |   1 |
| OP_STAR               | *                |    1 |   8 |
| KEYWORD_FROM          | FROM             |    1 |  10 |
| IDENTIFIER            | users            |    1 |  15 |
| OP_SEMICOLON          | ;                |    1 |  20 |

--- Parse Tree ---
|-- QUERY
  |-- SELECT_CLAUSE: "SELECT"
    |-- COLUMN_LIST
      |-- COLUMN: "*"
  |-- FROM_CLAUSE: "FROM"
    |-- TABLE_NAME: "users"

Semantic Analysis: SUCCESS
SELECT * - All columns selected.

╔═══════════════════════════════════════╗
║   QUERY VALIDATION: SUCCESSFUL        ║
╚═══════════════════════════════════════╝
```

---

## 3. Syntax Error - Missing FROM

### Input
```sql
SELECT * employees;
```

### Console Output
```
========================================
   PHASE 1: LEXICAL ANALYSIS
========================================
Input Query: SELECT * employees;

Lexical Analysis: SUCCESS
Total Tokens Generated: 4

--- Token Stream ---
| KEYWORD_SELECT        | SELECT           |    1 |   1 |
| OP_STAR               | *                |    1 |   8 |
| IDENTIFIER            | employees        |    1 |  10 |
| OP_SEMICOLON          | ;                |    1 |  19 |

========================================
   PHASE 2: SYNTAX ANALYSIS
========================================
Syntax Analysis: FAILED with 1 error(s)

========================================
   ERROR REPORT
========================================

[Error 1] Syntax Error at Line 1, Column 10: Expected 'FROM' keyword (found 'employees')
  1 | SELECT * employees;
    |          ^~~~

========================================
   COMPILATION SUMMARY
========================================

+-------------------+----------------+
| Phase             | Status         |
+-------------------+----------------+
| Lexical Analysis  | PASSED         |
| Syntax Analysis   | FAILED (1 err) |
| Semantic Analysis | SKIPPED        |
+-------------------+----------------+

╔═══════════════════════════════════════╗
║   QUERY VALIDATION: FAILED            ║
║   Total errors found: 1               ║
╚═══════════════════════════════════════╝
```

---

## 4. Lexical Error - Invalid Character

### Input
```sql
SELECT @ FROM users;
```

### Console Output
```
========================================
   PHASE 1: LEXICAL ANALYSIS
========================================
Input Query: SELECT @ FROM users;

Lexical Analysis: FAILED with 1 error(s)

========================================
   ERROR REPORT
========================================

[Error 1] Lexical Error at Line 1, Column 8: Unexpected character '@'
  1 | SELECT @ FROM users;
    |        ^~~~

========================================
   COMPILATION SUMMARY
========================================

+-------------------+----------------+
| Phase             | Status         |
+-------------------+----------------+
| Lexical Analysis  | FAILED (1 err) |
| Syntax Analysis   | SKIPPED        |
| Semantic Analysis | SKIPPED        |
+-------------------+----------------+

╔═══════════════════════════════════════╗
║   QUERY VALIDATION: FAILED            ║
║   Total errors found: 1               ║
╚═══════════════════════════════════════╝
```

---

## 5. Semantic Error - Non-existent Table

### Input
```sql
SELECT * FROM customers;
```

### Console Output
```
========================================
   PHASE 1: LEXICAL ANALYSIS
========================================
Lexical Analysis: SUCCESS

========================================
   PHASE 2: SYNTAX ANALYSIS
========================================
Syntax Analysis: SUCCESS

========================================
   PHASE 3: SEMANTIC ANALYSIS
========================================
Semantic Analysis: FAILED with 1 error(s)

========================================
   ERROR REPORT
========================================

[Error 1] Semantic Error at Line 1, Column 1: Table 'customers' does not exist. Available tables: employees, departments, users, products
  1 | SELECT * FROM customers;
    | ^~~~

========================================
   COMPILATION SUMMARY
========================================

+-------------------+----------------+
| Phase             | Status         |
+-------------------+----------------+
| Lexical Analysis  | PASSED         |
| Syntax Analysis   | PASSED         |
| Semantic Analysis | FAILED (1 err) |
+-------------------+----------------+

╔═══════════════════════════════════════╗
║   QUERY VALIDATION: FAILED            ║
║   Total errors found: 1               ║
╚═══════════════════════════════════════╝
```

---

## 6. Semantic Error - Invalid Column

### Input
```sql
SELECT invalid_column FROM employees;
```

### Console Output
```
========================================
   PHASE 3: SEMANTIC ANALYSIS
========================================
Table 'employees' validated.
Semantic Analysis: FAILED with 1 error(s)

========================================
   ERROR REPORT
========================================

[Error 1] Semantic Error at Line 1, Column 1: Column 'invalid_column' does not exist in table 'employees'. Available columns: id, name, age, salary, department
  1 | SELECT invalid_column FROM employees;
    | ^~~~

╔═══════════════════════════════════════╗
║   QUERY VALIDATION: FAILED            ║
║   Total errors found: 1               ║
╚═══════════════════════════════════════╝
```

---

## 7. Interactive Mode Session

```
╔══════════════════════════════════════════════════════════════╗
║          MINI SQL COMPILER FOR QUERY VALIDATION              ║
╚══════════════════════════════════════════════════════════════╝

Entering Interactive Mode...
Type SQL queries to validate. Type 'exit' or 'quit' to stop.
Type 'help' for syntax help, 'tables' to see available tables.

sql> tables

--- Symbol Table (Database Schema) ---
+------------------+------------------+----------+
| Table            | Column           | Type     |
+------------------+------------------+----------+
| employees        | id               | INT      |
|                  | name             | VARCHAR  |
|                  | age              | INT      |
|                  | salary           | FLOAT    |
|                  | department       | VARCHAR  |
+------------------+------------------+----------+
| departments      | id               | INT      |
|                  | name             | VARCHAR  |
|                  | budget           | FLOAT    |
+------------------+------------------+----------+
| users            | id               | INT      |
|                  | username         | VARCHAR  |
|                  | email            | VARCHAR  |
|                  | age              | INT      |
|                  | status           | VARCHAR  |
+------------------+------------------+----------+
| products         | id               | INT      |
|                  | name             | VARCHAR  |
|                  | price            | FLOAT    |
|                  | quantity         | INT      |
+------------------+------------------+----------+

sql> SELECT name, price FROM products;

[... compilation output ...]

╔═══════════════════════════════════════╗
║   QUERY VALIDATION: SUCCESSFUL        ║
╚═══════════════════════════════════════╝

sql> exit
Goodbye!
```

---

## Summary of Test Cases

| # | Query | Expected Result | Actual Result |
|---|-------|-----------------|---------------|
| 1 | `SELECT * FROM employees;` | VALID | ✅ PASSED |
| 2 | `SELECT name, age FROM users WHERE age > 25;` | VALID | ✅ PASSED |
| 3 | `SELECT * employees;` | SYNTAX ERROR | ✅ DETECTED |
| 4 | `SELECT @ FROM users;` | LEXICAL ERROR | ✅ DETECTED |
| 5 | `SELECT * FROM customers;` | SEMANTIC ERROR | ✅ DETECTED |
| 6 | `SELECT invalid FROM employees;` | SEMANTIC ERROR | ✅ DETECTED |
| 7 | `SELECT * FROM users` | SYNTAX ERROR (no ;) | ✅ DETECTED |
