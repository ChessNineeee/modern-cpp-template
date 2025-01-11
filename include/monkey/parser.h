//
// Created by wuzhengqi on 24-12-1.
//

#ifndef PARSER_H
#define PARSER_H
#include <monkey/ast.h>

#include <functional>
#include <memory>

using PrefixParsingFunction = std::function<Expression()>;
using InfixParsingFunction = std::function<Expression(Expression const&)>;

enum class Precedence
{
  LOWEST,
  EQUALS,
  LESSGREATER,
  SUM,
  PRODUCT,
  PREFIX,
  CALL
};

class Parser
{
 public:
  Parser(std::unique_ptr<Lexer> lexer);

  void ParseProgram();
  [[nodiscard]] std::shared_ptr<Program> GetProgram() const;
  ~Parser();

 private:
  struct ParserImpl;
  std::unique_ptr<ParserImpl> ParserImpl_;
};
#endif  // PARSER_H
