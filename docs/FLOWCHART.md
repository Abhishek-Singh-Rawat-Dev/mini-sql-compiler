# Mini SQL Compiler - Flowchart & Problem Statement

## Problem Statement

Design and implement a **Mini SQL Compiler** that validates SQL SELECT queries by simulating the core phases of a compiler. The compiler should:

1. **Tokenize** SQL queries into meaningful tokens (keywords, identifiers, operators)
2. **Parse** the token stream to verify grammatical correctness
3. **Validate** semantic correctness (table/column existence)
4. **Report** meaningful errors with precise location information
5. **Generate** a Parse Tree as intermediate representation

**Scope:** The compiler validates queries without executing them on a real database.

**Supported SQL Subset:**
```sql
SELECT column1, column2, ... | *
FROM table_name
[WHERE column operator value]
;
```

---

## System Flowchart

```mermaid
flowchart TD
    subgraph INPUT
        A[/"SQL Query Input"/]
    end

    subgraph PHASE1["PHASE 1: LEXICAL ANALYSIS"]
        B["Read Character Stream"]
        C{"Is Valid Character?"}
        D["Identify Token Type"]
        E["Create Token with Position"]
        F["Add to Token Stream"]
        G[("Lexical Error")]
    end

    subgraph PHASE2["PHASE 2: SYNTAX ANALYSIS"]
        H["Read Token Stream"]
        I["Apply Grammar Rules"]
        J{"Matches Grammar?"}
        K["Build Parse Tree Node"]
        L["Recursive Descent"]
        M[("Syntax Error")]
    end

    subgraph PHASE3["PHASE 3: SEMANTIC ANALYSIS"]
        N["Traverse Parse Tree"]
        O["Lookup Symbol Table"]
        P{"Table Exists?"}
        Q{"Columns Valid?"}
        R["Validate Condition"]
        S[("Semantic Error")]
    end

    subgraph OUTPUT
        T["Error Handler"]
        U{{"Any Errors?"}}
        V[/"VALID: Query Accepted"/]
        W[/"INVALID: Error Report"/]
    end

    A --> B
    B --> C
    C -->|Yes| D
    C -->|No| G
    D --> E
    E --> F
    F -->|More chars| B
    F -->|Done| H
    G --> T

    H --> I
    I --> J
    J -->|Yes| K
    J -->|No| M
    K --> L
    L -->|More rules| I
    L -->|Done| N
    M --> T

    N --> O
    O --> P
    P -->|Yes| Q
    P -->|No| S
    Q -->|Yes| R
    Q -->|No| S
    R --> T
    S --> T

    T --> U
    U -->|No| V
    U -->|Yes| W

    style A fill:#e1f5fe
    style V fill:#c8e6c9
    style W fill:#ffcdd2
    style G fill:#ffcdd2
    style M fill:#ffcdd2
    style S fill:#ffcdd2
```

---

## Detailed Phase Flow

### Phase 1: Lexical Analysis (Tokenization)

```mermaid
flowchart LR
    subgraph Input
        A["SELECT name FROM users;"]
    end
    
    subgraph Lexer
        B["Character Scanner"]
        C["Pattern Matcher"]
        D["Keyword Table Lookup"]
    end
    
    subgraph Output
        E["Token Stream"]
    end
    
    A --> B --> C --> D --> E
    
    E --- F["[KEYWORD_SELECT]"]
    E --- G["[IDENTIFIER: name]"]
    E --- H["[KEYWORD_FROM]"]
    E --- I["[IDENTIFIER: users]"]
    E --- J["[OP_SEMICOLON]"]
```

### Phase 2: Syntax Analysis (Parsing)

```mermaid
flowchart TD
    subgraph Grammar["Grammar Rules"]
        R1["query → SELECT column_list FROM table_name [WHERE condition] ;"]
        R2["column_list → * | column {, column}"]
        R3["condition → column operator value"]
    end
    
    subgraph Parser["Recursive Descent Parser"]
        A["parseQuery()"]
        B["parseSelectClause()"]
        C["parseColumnList()"]
        D["parseFromClause()"]
        E["parseWhereClause()"]
        F["parseCondition()"]
    end
    
    subgraph Tree["Parse Tree"]
        T1["QUERY"]
        T2["SELECT_CLAUSE"]
        T3["COLUMN_LIST"]
        T4["FROM_CLAUSE"]
        T5["TABLE_NAME"]
    end
    
    A --> B --> C
    A --> D --> T4 --> T5
    A --> E --> F
    
    T1 --> T2 --> T3
    T1 --> T4
```

### Phase 3: Semantic Analysis

```mermaid
flowchart TD
    subgraph SymbolTable["Symbol Table (Schema)"]
        ST1["employees: id, name, age, salary"]
        ST2["users: id, username, email, age"]
        ST3["products: id, name, price, quantity"]
    end
    
    subgraph Validation["Semantic Checks"]
        A["Extract Table Name"]
        B{"Table in Schema?"}
        C["Extract Column Names"]
        D{"Columns in Table?"}
        E["Check WHERE Column"]
        F{"Valid Condition?"}
    end
    
    subgraph Result
        G["✓ Semantic Valid"]
        H["✗ Semantic Error"]
    end
    
    A --> B
    B -->|Yes| C
    B -->|No| H
    C --> D
    D -->|Yes| E
    D -->|No| H
    E --> F
    F -->|Yes| G
    F -->|No| H
```

---

## Data Flow Diagram

```mermaid
flowchart LR
    subgraph User
        U["User Input"]
    end
    
    subgraph Compiler["Mini SQL Compiler"]
        L["Lexer<br/>(Member 1)"]
        P["Parser<br/>(Member 2)"]
        S["Semantic<br/>(Member 3)"]
        E["Error Handler<br/>(Member 4)"]
    end
    
    subgraph Outputs
        O1["Token Stream"]
        O2["Parse Tree"]
        O3["Validation Result"]
        O4["Error Report"]
    end
    
    U -->|SQL Query| L
    L -->|Tokens| P
    L -.->|Lexical Errors| E
    P -->|Parse Tree| S
    P -.->|Syntax Errors| E
    S -->|Result| E
    S -.->|Semantic Errors| E
    
    L --> O1
    P --> O2
    S --> O3
    E --> O4
```

---

## Error Handling Flow

```mermaid
flowchart TD
    subgraph Errors["Error Types"]
        E1["Lexical Error<br/>Invalid character '@'"]
        E2["Syntax Error<br/>Missing FROM keyword"]
        E3["Semantic Error<br/>Table doesn't exist"]
    end
    
    subgraph Handler["Error Handler"]
        H1["Collect Errors"]
        H2["Format Message"]
        H3["Add Location Info"]
        H4["Generate Report"]
    end
    
    subgraph Output["Console Output"]
        O["Error at Line 1, Col 8:<br/>Unexpected character '@'<br/>  1 │ SELECT @ FROM users;<br/>    │        ^~~~"]
    end
    
    E1 --> H1
    E2 --> H1
    E3 --> H1
    H1 --> H2 --> H3 --> H4 --> O
```

---

## Summary

| Phase | Input | Output | Key Operation |
|-------|-------|--------|---------------|
| Lexical | Character stream | Token stream | Pattern matching |
| Syntax | Token stream | Parse tree | Recursive descent |
| Semantic | Parse tree | Validation result | Symbol table lookup |
| Output | All results | Final report | Error aggregation |
