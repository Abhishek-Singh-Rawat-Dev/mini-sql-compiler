/**
 * Mini SQL Compiler for Query Validation
 * =======================================
 * File: parser.cpp
 * Description: Syntax Analyzer Implementation
 * Team Member: Member 2
 *
 * THEORY:
 * -------
 * This file implements a RECURSIVE DESCENT PARSER for SQL queries.
 *
 * Recursive Descent Parsing:
 * - A top-down parsing technique
 * - Each non-terminal in the grammar has a corresponding parsing function
 * - The parser starts from the start symbol and recursively matches rules
 * - It's called "predictive" because we can determine which rule to apply
 *   by looking at the current token (lookahead)
 *
 * Parse Tree Construction:
 * - As we match grammar rules, we build a tree structure
 * - Each node represents a grammar production
 * - Leaf nodes contain actual token values
 * - The tree captures the syntactic structure of the query
 *
 * Error Recovery:
 * - When a syntax error is detected, we report it and try to continue
 * - "Synchronization" helps the parser recover to a known state
 * - This allows reporting multiple errors in one pass
 */

#include "../include/parser.h"
#include <iostream>

namespace MiniSQL {

// Constructor
Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), current(0) {}

// ============================================================================
// MAIN PARSING METHOD
// ============================================================================
ParseTree Parser::parse() {
  std::cout << "\n========================================\n";
  std::cout << "   PHASE 2: SYNTAX ANALYSIS\n";
  std::cout << "========================================\n";

  ParseTree tree = parseQuery();

  if (errors.empty() && tree != nullptr) {
    std::cout << "Syntax Analysis: SUCCESS\n";
    std::cout << "Parse Tree constructed successfully.\n";
  } else {
    std::cout << "Syntax Analysis: FAILED with " << errors.size()
              << " error(s)\n";
  }

  return tree;
}

// ============================================================================
// GRAMMAR RULE: <query> ::= SELECT <column_list> FROM <table_name>
// [<where_clause>] ;
// ============================================================================
ParseTree Parser::parseQuery() {
  auto queryNode = std::make_shared<ParseTreeNode>(NodeType::QUERY);

  // Parse SELECT clause
  auto selectClause = parseSelectClause();
  if (selectClause) {
    queryNode->addChild(selectClause);
  } else {
    return nullptr;
  }

  // Parse FROM clause
  auto fromClause = parseFromClause();
  if (fromClause) {
    queryNode->addChild(fromClause);
  } else {
    return nullptr;
  }

  // Parse optional WHERE clause
  if (check(TokenType::KEYWORD_WHERE)) {
    auto whereClause = parseWhereClause();
    if (whereClause) {
      queryNode->addChild(whereClause);
    } else {
      return nullptr;
    }
  }

  // Expect semicolon at the end
  consume(TokenType::OP_SEMICOLON, "Expected ';' at end of query");

  return queryNode;
}

// ============================================================================
// GRAMMAR RULE: SELECT <column_list>
// ============================================================================
ParseTree Parser::parseSelectClause() {
  // Expect SELECT keyword
  if (!match(TokenType::KEYWORD_SELECT)) {
    error("Expected 'SELECT' keyword at beginning of query");
    return nullptr;
  }

  auto selectNode =
      std::make_shared<ParseTreeNode>(NodeType::SELECT_CLAUSE, "SELECT");

  // Parse column list
  auto columnList = parseColumnList();
  if (columnList) {
    selectNode->addChild(columnList);
  } else {
    return nullptr;
  }

  return selectNode;
}

// ============================================================================
// GRAMMAR RULE: <column_list> ::= * | <column_name> { , <column_name> }*
// ============================================================================
ParseTree Parser::parseColumnList() {
  auto columnListNode = std::make_shared<ParseTreeNode>(NodeType::COLUMN_LIST);

  // Check for SELECT *
  if (match(TokenType::OP_STAR)) {
    auto starColumn = std::make_shared<ParseTreeNode>(NodeType::COLUMN, "*");
    columnListNode->addChild(starColumn);
    return columnListNode;
  }

  // Parse first column name
  if (!check(TokenType::IDENTIFIER)) {
    error("Expected column name or '*' after SELECT");
    return nullptr;
  }

  // Add first column
  Token colToken = advance();
  auto column =
      std::make_shared<ParseTreeNode>(NodeType::COLUMN, colToken.value);
  columnListNode->addChild(column);

  // Parse additional columns separated by commas
  while (match(TokenType::OP_COMMA)) {
    if (!check(TokenType::IDENTIFIER)) {
      error("Expected column name after ','");
      return nullptr;
    }
    Token nextCol = advance();
    auto nextColumn =
        std::make_shared<ParseTreeNode>(NodeType::COLUMN, nextCol.value);
    columnListNode->addChild(nextColumn);
  }

  return columnListNode;
}

// ============================================================================
// GRAMMAR RULE: FROM <table_name>
// ============================================================================
ParseTree Parser::parseFromClause() {
  if (!match(TokenType::KEYWORD_FROM)) {
    error("Expected 'FROM' keyword");
    return nullptr;
  }

  auto fromNode =
      std::make_shared<ParseTreeNode>(NodeType::FROM_CLAUSE, "FROM");

  if (!check(TokenType::IDENTIFIER)) {
    error("Expected table name after 'FROM'");
    return nullptr;
  }

  Token tableToken = advance();
  auto tableName =
      std::make_shared<ParseTreeNode>(NodeType::TABLE_NAME, tableToken.value);
  fromNode->addChild(tableName);

  return fromNode;
}

// ============================================================================
// GRAMMAR RULE: WHERE <condition>
// ============================================================================
ParseTree Parser::parseWhereClause() {
  if (!match(TokenType::KEYWORD_WHERE)) {
    return nullptr;
  }

  auto whereNode =
      std::make_shared<ParseTreeNode>(NodeType::WHERE_CLAUSE, "WHERE");

  auto condition = parseCondition();
  if (condition) {
    whereNode->addChild(condition);
  } else {
    return nullptr;
  }

  return whereNode;
}

// ============================================================================
// GRAMMAR RULE: <condition> ::= <column_name> <rel_op> <value>
// ============================================================================
ParseTree Parser::parseCondition() {
  auto conditionNode = std::make_shared<ParseTreeNode>(NodeType::CONDITION);

  // Left operand: column name
  if (!check(TokenType::IDENTIFIER)) {
    error("Expected column name in WHERE condition");
    return nullptr;
  }
  Token colToken = advance();
  auto column =
      std::make_shared<ParseTreeNode>(NodeType::COLUMN, colToken.value);
  conditionNode->addChild(column);

  // Relational operator
  if (!check(TokenType::OP_EQUALS) && !check(TokenType::OP_LESS_THAN) &&
      !check(TokenType::OP_GREATER_THAN)) {
    error("Expected relational operator (=, <, >) in condition");
    return nullptr;
  }
  Token opToken = advance();
  auto opNode =
      std::make_shared<ParseTreeNode>(NodeType::OPERATOR, opToken.value);
  conditionNode->addChild(opNode);

  // Right operand: value (identifier, number, or string)
  if (!check(TokenType::IDENTIFIER) && !check(TokenType::NUMBER) &&
      !check(TokenType::STRING_LITERAL)) {
    error("Expected value (identifier, number, or string) in condition");
    return nullptr;
  }
  Token valueToken = advance();
  auto valueNode =
      std::make_shared<ParseTreeNode>(NodeType::VALUE, valueToken.value);
  conditionNode->addChild(valueNode);

  return conditionNode;
}

// ============================================================================
// TOKEN NAVIGATION HELPERS
// ============================================================================

Token Parser::peek() const { return tokens[current]; }

Token Parser::previous() const { return tokens[current - 1]; }

Token Parser::advance() {
  if (!isAtEnd())
    current++;
  return previous();
}

bool Parser::isAtEnd() const { return peek().type == TokenType::END_OF_INPUT; }

bool Parser::check(TokenType type) const {
  if (isAtEnd())
    return false;
  return peek().type == type;
}

bool Parser::match(TokenType type) {
  if (check(type)) {
    advance();
    return true;
  }
  return false;
}

Token Parser::consume(TokenType type, const std::string &message) {
  if (check(type))
    return advance();
  error(message);
  return Token(TokenType::UNKNOWN, "", peek().line, peek().column);
}

// ============================================================================
// ERROR HANDLING
// ============================================================================

void Parser::error(const std::string &message) {
  Token token = peek();
  errors.push_back(CompilerError(ErrorType::SYNTAX_ERROR,
                                 message + " (found '" + token.value + "')",
                                 token.line, token.column));
}

void Parser::synchronize() {
  advance();

  while (!isAtEnd()) {
    if (previous().type == TokenType::OP_SEMICOLON)
      return;

    switch (peek().type) {
    case TokenType::KEYWORD_SELECT:
      return;
    default:
      break;
    }
    advance();
  }
}

bool Parser::hasErrors() const { return !errors.empty(); }

std::vector<CompilerError> Parser::getErrors() const { return errors; }

// ============================================================================
// PARSE TREE VISUALIZATION
// ============================================================================
void Parser::printParseTree(const ParseTree &node, int indent) {
  if (!node)
    return;

  // Print indentation
  for (int i = 0; i < indent; i++) {
    std::cout << "  ";
  }

  // Print node
  std::cout << "|-- " << nodeTypeToString(node->type);
  if (!node->value.empty()) {
    std::cout << ": \"" << node->value << "\"";
  }
  std::cout << "\n";

  // Recursively print children
  for (const auto &child : node->children) {
    printParseTree(child, indent + 1);
  }
}

} // namespace MiniSQL
