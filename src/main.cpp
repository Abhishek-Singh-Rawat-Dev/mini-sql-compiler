/**
 * ============================================================================
 *                    MINI SQL COMPILER FOR QUERY VALIDATION
 * ============================================================================
 *
 * Project: Compiler Design Academic Project
 * Subject: Compiler Design
 * Team Size: 4 Members
 *
 * Team Members and Responsibilities:
 * ----------------------------------
 * Member 1: Lexical Analyzer (lexer.h, lexer.cpp)
 * Member 2: Syntax Analyzer  (parser.h, parser.cpp)
 * Member 3: Semantic Analyzer & Symbol Table (semantic.h/cpp,
 * symbol_table.h/cpp) Member 4: Error Handler, Main Driver, Testing
 * (error_handler.h/cpp, main.cpp)
 *
 * Compilation Phases Implemented:
 * ------------------------------
 * 1. LEXICAL ANALYSIS: Converts character stream to token stream
 * 2. SYNTAX ANALYSIS: Validates grammar and builds parse tree
 * 3. SEMANTIC ANALYSIS: Validates meaning and resolves identifiers
 * 4. ERROR HANDLING: Reports errors from all phases
 *
 * Supported SQL Subset:
 * --------------------
 * SELECT column1, column2, ... | *
 * FROM table_name
 * [WHERE column operator value]
 * ;
 *
 * Operators: =, <, >
 *
 * Usage:
 * ------
 * ./sql_compiler                    (Interactive mode)
 * echo "SELECT * FROM users;" | ./sql_compiler   (Pipe mode)
 *
 * ============================================================================
 */

#include "../include/error_handler.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/semantic.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace MiniSQL;

// Function prototypes
void printBanner();
void printHelp();
void compileQuery(const std::string &query);
void runInteractiveMode();
void runDemoMode();

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main(int argc, char *argv[]) {
  printBanner();

  // Check for command line arguments
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "--help" || arg == "-h") {
      printHelp();
      return 0;
    } else if (arg == "--demo") {
      runDemoMode();
      return 0;
    }
  }

  // Interactive mode
  runInteractiveMode();

  return 0;
}

// ============================================================================
// BANNER AND HELP
// ============================================================================
void printBanner() {
  std::cout << "\n";
  std::cout
      << "╔══════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║          MINI SQL COMPILER FOR QUERY VALIDATION              ║\n";
  std::cout
      << "║                   Compiler Design Project                    ║\n";
  std::cout
      << "╠══════════════════════════════════════════════════════════════╣\n";
  std::cout
      << "║  Team Members:                                               ║\n";
  std::cout
      << "║    Member 1: Lexical Analyzer                                ║\n";
  std::cout
      << "║    Member 2: Syntax Analyzer                                 ║\n";
  std::cout
      << "║    Member 3: Semantic Analyzer                               ║\n";
  std::cout
      << "║    Member 4: Error Handler & Testing                         ║\n";
  std::cout
      << "╚══════════════════════════════════════════════════════════════╝\n";
}

void printHelp() {
  std::cout << "\nUsage: ./sql_compiler [options]\n";
  std::cout << "\nOptions:\n";
  std::cout << "  --help, -h    Show this help message\n";
  std::cout << "  --demo        Run demo with sample queries\n";
  std::cout << "\nSupported SQL Syntax:\n";
  std::cout
      << "  SELECT column1, column2 | * FROM table_name [WHERE condition];\n";
  std::cout << "\nAvailable Tables (for semantic testing):\n";
  std::cout << "  employees (id, name, age, salary, department)\n";
  std::cout << "  departments (id, name, budget)\n";
  std::cout << "  users (id, username, email, age, status)\n";
  std::cout << "  products (id, name, price, quantity)\n";
}

// ============================================================================
// QUERY COMPILATION - Main compilation pipeline
// ============================================================================
void compileQuery(const std::string &query) {
  ErrorHandler errorHandler;
  errorHandler.setSource(query);

  bool syntaxValid = false;
  bool semanticValid = false;
  ParseTree parseTree = nullptr;

  std::cout << "\n══════════════════════════════════════════\n";
  std::cout << "Starting compilation of query...\n";
  std::cout << "══════════════════════════════════════════\n";

  // ========================================
  // PHASE 1: LEXICAL ANALYSIS (Member 1)
  // ========================================
  Lexer lexer(query);
  std::vector<Token> tokens = lexer.tokenize();

  // Display token stream
  lexer.printTokens();

  // Check for lexical errors
  if (lexer.hasErrors()) {
    errorHandler.addErrors(lexer.getErrors());
    errorHandler.printErrors();
    errorHandler.printSummary(false, false);
    return;
  }

  // ========================================
  // PHASE 2: SYNTAX ANALYSIS (Member 2)
  // ========================================
  Parser parser(tokens);
  parseTree = parser.parse();

  // Check for syntax errors
  if (parser.hasErrors()) {
    errorHandler.addErrors(parser.getErrors());
    errorHandler.printErrors();
    errorHandler.printSummary(false, false);
    return;
  }

  syntaxValid = (parseTree != nullptr);

  // Display parse tree
  if (parseTree) {
    std::cout << "\n--- Parse Tree (Intermediate Representation) ---\n";
    Parser::printParseTree(parseTree);
  }

  // ========================================
  // PHASE 3: SEMANTIC ANALYSIS (Member 3)
  // ========================================
  SemanticAnalyzer semanticAnalyzer;

  // Optionally print symbol table
  std::cout << "\n--- Available Schema for Validation ---\n";
  semanticAnalyzer.printSymbolTable();

  semanticValid = semanticAnalyzer.analyze(parseTree);

  // Check for semantic errors
  if (semanticAnalyzer.hasErrors()) {
    errorHandler.addErrors(semanticAnalyzer.getErrors());
  }

  // ========================================
  // PHASE 4: FINAL OUTPUT (Member 4)
  // ========================================
  if (errorHandler.hasErrors()) {
    errorHandler.printErrors();
  }

  errorHandler.printSummary(syntaxValid, semanticValid);
}

// ============================================================================
// INTERACTIVE MODE
// ============================================================================
void runInteractiveMode() {
  std::cout << "\nEntering Interactive Mode...\n";
  std::cout << "Type SQL queries to validate. Type 'exit' or 'quit' to stop.\n";
  std::cout
      << "Type 'help' for syntax help, 'tables' to see available tables.\n";

  std::string line;
  std::string query;

  while (true) {
    std::cout << "\nsql> ";

    if (!std::getline(std::cin, line)) {
      break; // EOF
    }

    // Trim whitespace
    size_t start = line.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
      continue;
    line = line.substr(start);

    // Check for commands
    if (line == "exit" || line == "quit") {
      std::cout << "Goodbye!\n";
      break;
    }

    if (line == "help") {
      printHelp();
      continue;
    }

    if (line == "tables") {
      SemanticAnalyzer temp;
      temp.printSymbolTable();
      continue;
    }

    if (line == "demo") {
      runDemoMode();
      continue;
    }

    if (line == "clear") {
      system("clear");
      printBanner();
      continue;
    }

    // Accumulate query until semicolon
    query += line;

    if (query.find(';') != std::string::npos) {
      compileQuery(query);
      query.clear();
    } else {
      std::cout << "     (continue query, end with ';')\n";
    }
  }
}

// ============================================================================
// DEMO MODE - Run sample queries
// ============================================================================
void runDemoMode() {
  std::cout << "\n";
  std::cout
      << "╔══════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║                    DEMO MODE - Sample Queries                ║\n";
  std::cout
      << "╚══════════════════════════════════════════════════════════════╝\n";

  std::vector<std::pair<std::string, std::string>> testCases = {
      {"Valid SELECT * query", "SELECT * FROM employees;"},

      {"Valid SELECT with specific columns",
       "SELECT name, age, salary FROM employees;"},

      {"Valid SELECT with WHERE clause",
       "SELECT username, email FROM users WHERE age > 25;"},

      {"Syntax Error - Missing FROM keyword", "SELECT * employees;"},

      {"Syntax Error - Missing semicolon", "SELECT * FROM users"},

      {"Lexical Error - Invalid character", "SELECT @ FROM users;"},

      {"Semantic Error - Non-existent table", "SELECT * FROM customers;"},

      {"Semantic Error - Invalid column",
       "SELECT invalid_col FROM employees;"}};

  for (size_t i = 0; i < testCases.size(); i++) {
    std::cout << "\n";
    std::cout
        << "┌────────────────────────────────────────────────────────────┐\n";
    std::cout << "│ Test Case " << (i + 1) << ": " << testCases[i].first;
    for (size_t j = testCases[i].first.length() + 15; j < 60; j++)
      std::cout << " ";
    std::cout << "│\n";
    std::cout << "│ Query: " << testCases[i].second;
    for (size_t j = testCases[i].second.length() + 8; j < 60; j++)
      std::cout << " ";
    std::cout << "│\n";
    std::cout
        << "└────────────────────────────────────────────────────────────┘\n";

    compileQuery(testCases[i].second);

    std::cout << "\n--- Press Enter to continue ---";
    std::cin.get();
  }

  std::cout
      << "\n╔══════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║                     Demo Complete!                           ║\n";
  std::cout
      << "╚══════════════════════════════════════════════════════════════╝\n";
}
