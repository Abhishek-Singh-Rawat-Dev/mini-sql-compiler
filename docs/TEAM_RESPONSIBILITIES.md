# Team Responsibilities - Difficulty-Based Division

## Mini SQL Compiler Project

This document assigns work to 4 team members based on **difficulty level** - from easiest to hardest.

---

## 📊 Difficulty Overview

| Member | Role | Difficulty | Work Hours Est. |
|--------|------|------------|-----------------|
| **Member 4** | Error Handler & Testing | ⭐ Easy | 8-10 hrs |
| **Member 1** | Lexical Analyzer | ⭐⭐ Medium | 12-15 hrs |
| **Member 3** | Semantic Analyzer | ⭐⭐⭐ Medium-Hard | 15-18 hrs |
| **Member 2** | Syntax Analyzer | ⭐⭐⭐⭐ Hard | 18-22 hrs |

---

## Member 4: Error Handler & Testing (⭐ EASY)

### Why Easy?
- Straightforward string manipulation
- No complex algorithms
- Collects and formats errors from other modules
- Testing involves running predefined queries

### Files to Work On
- `include/error_handler.h`
- `src/error_handler.cpp`
- `src/main.cpp` (driver program)
- `tests/valid_queries.txt`
- `tests/invalid_queries.txt`
- All documentation files

### Key Concepts to Understand
1. Error structure (type, message, line, column)
2. String formatting for console output
3. Basic C++ classes and vectors
4. How to integrate with other modules

### Main Tasks
```
[ ] Create ErrorHandler class
[ ] Implement addError() and addErrors() methods
[ ] Format error messages with source line display
[ ] Create compilation summary with ASCII art
[ ] Write main.cpp driver integrating all phases
[ ] Create test query files
[ ] Write documentation (README, sample outputs)
```

### Code Complexity: LOW
- Simple data storage and retrieval
- Printf/cout formatting
- No parsing or complex logic

---

## Member 1: Lexical Analyzer (⭐⭐ MEDIUM)

### Why Medium?
- Character-by-character processing (systematic)
- Pattern recognition using switch-case
- Keyword table lookup (simple hash map)
- Well-defined token categories

### Files to Work On
- `include/common.h` (Token types)
- `include/lexer.h`
- `src/lexer.cpp`

### Key Concepts to Understand
1. **Tokens** - Smallest meaningful units
2. **Lexemes** - Actual character sequences
3. **Finite Automata** - State-based pattern matching
4. **Keyword Table** - Reserved word lookup

### Main Tasks
```
[ ] Define TokenType enum (SELECT, FROM, IDENTIFIER, etc.)
[ ] Define Token structure with position info
[ ] Implement character scanning (advance, peek)
[ ] Implement scanToken() switch-case logic
[ ] Implement scanIdentifier() for keywords/identifiers
[ ] Implement scanNumber() for numeric literals
[ ] Implement scanString() for string literals
[ ] Create tokenize() main method
[ ] Print token stream in tabular format
```

### Code Complexity: MEDIUM
- Sequential character processing
- State tracking (current position, line, column)
- Pattern matching with simple rules

---

## Member 3: Semantic Analyzer (⭐⭐⭐ MEDIUM-HARD)

### Why Medium-Hard?
- Requires understanding parse tree structure
- Symbol table design and lookup
- Multiple validation checks
- Must understand context (which table, which columns)

### Files to Work On
- `include/symbol_table.h`
- `src/symbol_table.cpp`
- `include/semantic.h`
- `src/semantic.cpp`

### Key Concepts to Understand
1. **Symbol Table** - Database of identifiers
2. **Name Resolution** - Finding if identifiers exist
3. **Type Checking** - Validating compatible operations
4. **Attribute Grammar** - Passing info through parse tree

### Main Tasks
```
[ ] Design TableInfo and ColumnInfo structures
[ ] Create SymbolTable class with lookup methods
[ ] Initialize sample database schema
[ ] Implement tableExists() and columnExists()
[ ] Create SemanticAnalyzer class
[ ] Validate FROM clause first (establish context)
[ ] Validate SELECT columns against table schema
[ ] Validate WHERE clause column and condition
[ ] Handle "*" (select all) case
[ ] Generate meaningful semantic error messages
```

### Code Complexity: MEDIUM-HIGH
- Tree traversal logic
- Context management (current table)
- Multiple validation rules
- Coordination with parser output

---

## Member 2: Syntax Analyzer (⭐⭐⭐⭐ HARD)

### Why Hard?
- Must understand Context-Free Grammar (CFG)
- Recursive descent implementation
- Parse tree construction
- Error recovery handling
- Most theoretical knowledge required

### Files to Work On
- `include/common.h` (Parse tree nodes)
- `include/parser.h`
- `src/parser.cpp`

### Key Concepts to Understand
1. **Context-Free Grammar (CFG)** - Production rules
2. **Recursive Descent Parsing** - Top-down technique
3. **Parse Tree / AST** - Tree representation of syntax
4. **Lookahead** - Predicting which rule to apply
5. **FIRST and FOLLOW Sets** - Grammar analysis

### SQL Grammar to Implement
```
<query>        ::= SELECT <column_list> FROM <table_name> [<where_clause>] ;
<column_list>  ::= * | <column_name> { , <column_name> }*
<column_name>  ::= IDENTIFIER
<table_name>   ::= IDENTIFIER
<where_clause> ::= WHERE <condition>
<condition>    ::= <column_name> <rel_op> <value>
<rel_op>       ::= = | < | >
<value>        ::= IDENTIFIER | NUMBER | STRING_LITERAL
```

### Main Tasks
```
[ ] Understand and design the SQL grammar
[ ] Define ParseTreeNode structure
[ ] Implement token navigation (peek, advance, match, check)
[ ] Implement parseQuery() - top-level rule
[ ] Implement parseSelectClause() - SELECT keyword
[ ] Implement parseColumnList() - columns or *
[ ] Implement parseFromClause() - FROM + table
[ ] Implement parseWhereClause() - optional WHERE
[ ] Implement parseCondition() - column op value
[ ] Build parse tree during parsing
[ ] Implement error recovery (synchronize)
[ ] Implement printParseTree() for visualization
```

### Code Complexity: HIGH
- Recursive function calls matching grammar
- Tree construction during parsing
- Must handle all grammar cases
- Error recovery is tricky

---

## 📈 Learning Curve by Member

```
Member 4 (Easy)      ████░░░░░░░░░░░░░░░░ 20%
Member 1 (Medium)    ████████░░░░░░░░░░░░ 40%
Member 3 (Med-Hard)  ████████████░░░░░░░░ 60%
Member 2 (Hard)      ████████████████████ 100%
```

---

## 🔗 Dependency Order

Work should proceed in this order:

```
1. Member 1 (Lexer)     ─┬─→ Can start immediately
2. Member 4 (Errors)    ─┤   Can start in parallel
                         │
3. Member 2 (Parser)    ←┘   Needs tokens from Lexer
                         │
4. Member 3 (Semantic)  ←────Needs parse tree from Parser
```

### Integration Timeline
| Week | Member 1 | Member 2 | Member 3 | Member 4 |
|------|----------|----------|----------|----------|
| 1 | Lexer done | Grammar study | Symbol table | Error class |
| 2 | Testing | Parser impl | Wait | Main driver |
| 3 | Integration | Parse tree | Semantic impl | Testing |
| 4 | All: Final testing and documentation |

---

## 🎯 Viva Focus Areas by Member

**Member 4:** Error types, error recovery, testing methodologies
**Member 1:** Tokens, lexemes, finite automata, keyword recognition
**Member 3:** Symbol table, name resolution, type checking
**Member 2:** CFG, recursive descent, parse trees, grammar rules

---

## Summary

| Difficulty | Member | Key Skill Required |
|------------|--------|-------------------|
| ⭐ Easy | Member 4 | Basic C++, string handling |
| ⭐⭐ Medium | Member 1 | Pattern matching, state machines |
| ⭐⭐⭐ Med-Hard | Member 3 | Data structures, tree traversal |
| ⭐⭐⭐⭐ Hard | Member 2 | Grammar theory, recursion |
