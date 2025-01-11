//
// Created by wuzhengqi on 24-12-1.
//

#ifndef AST_H
#define AST_H
#include <monkey/lexer.h>

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

struct StringVisitor
{
  template<typename T>
  [[nodiscard]] std::string operator()(T const& t) const
  {
    return t.String();
  }
};

struct TokenLiteralVisitor
{
  template<typename T>
  [[nodiscard]] std::string operator()(T const& t) const
  {
    return t.TokenLiteral();
  }
};

struct ExpressionNodeVisitor
{
  template<typename T>
  void operator()(T& t)
  {
    t.ExpressionNode();
  }
};

struct Identifier;
struct IntegerLiteral;

struct PrefixExpression;
struct InfixExpression;

using Expression = std::variant<Identifier, IntegerLiteral, PrefixExpression, InfixExpression>;

struct IntegerLiteral
{
  Token Token_;
  int64_t Value_;

  [[nodiscard]] std::string TokenLiteral() const
  {
    return this->Token_.Literal_;
  }
  void ExpressionNode()
  {
  }

  [[nodiscard]] std::string String() const
  {
    return this->Token_.Literal_;
  }
};

struct Identifier
{
  [[nodiscard]] std::string TokenLiteral() const
  {
    return this->Token_.Literal_;
  }
  void ExpressionNode()
  {
  }

  [[nodiscard]] std::string String() const
  {
    return this->Value_;
  }

  Token Token_;
  std::string Value_;
};

using PrefixExpressionNode = std::variant<Identifier, IntegerLiteral, PrefixExpression>;

struct PrefixExpression
{
  [[nodiscard]] std::string TokenLiteral() const
  {
    return this->Token_.Literal_;
  }
  void ExpressionNode()
  {
  }

  [[nodiscard]] std::string String() const
  {
    auto ss = std::stringstream{};
    ss << "(";
    ss << this->Operator_;
    assert(this->Right_ != nullptr);
    ss << std::visit(StringVisitor{}, *this->Right_);
    ss << ")";
    return ss.str();
  }

  Token Token_;
  std::string Operator_;
  std::shared_ptr<PrefixExpressionNode> Right_;
};

struct InfixExpression
{
  [[nodiscard]] std::string TokenLiteral() const
  {
    return this->Token_.Literal_;
  }
  void ExpressionNode()
  {
  }

  [[nodiscard]] std::string String() const
  {
    auto ss = std::stringstream{};
    ss << "(";
    assert(this->Right_ != nullptr);
    ss << std::visit(StringVisitor{}, *this->Left_);
    ss << this->Operator_;
    assert(this->Right_ != nullptr);
    ss << std::visit(StringVisitor{}, *this->Right_);
    ss << ")";
    return ss.str();
  }

  Token Token_;
  std::shared_ptr<PrefixExpressionNode> Left_;
  std::string Operator_;
  std::shared_ptr<PrefixExpressionNode> Right_;
};

struct LetStatement
{
  [[nodiscard]] std::string TokenLiteral() const
  {
    return this->Token_.Literal_;
  }

  [[nodiscard]] std::string String() const
  {
    auto ss = std::stringstream{};
    ss << this->TokenLiteral() << " ";
    ss << this->Name_.String();
    ss << " = ";
    ss << std::visit(StringVisitor{}, this->Value_);
    ss << ";";
    return ss.str();
  }

  Token Token_;
  Identifier Name_;
  Expression Value_;
};

struct ReturnStatement
{
  [[nodiscard]] std::string TokenLiteral() const
  {
    return this->Token_.Literal_;
  }

  [[nodiscard]] std::string String() const
  {
    auto ss = std::stringstream{};
    ss << this->TokenLiteral() << " ";
    ss << std::visit(StringVisitor{}, this->ReturnValue_);
    ss << ";";
    return ss.str();
  }

  Token Token_;
  Expression ReturnValue_;
};

struct ExpressionStatement
{
  [[nodiscard]] std::string TokenLiteral() const
  {
    return this->Token_.Literal_;
  }

  [[nodiscard]] std::string String() const
  {
    auto ss = std::stringstream{};
    ss << std::visit(StringVisitor{}, this->Expression_);
    return ss.str();
  }

  Token Token_;
  Expression Expression_;
};

using Statement = std::variant<LetStatement, ReturnStatement, ExpressionStatement>;

struct StatementNodeVisitor
{
  template<typename T>
  void operator()(T& t)
  {
    t.StatementNode();
  }
};

struct Program
{
 public:
  [[nodiscard]] std::string TokenLiteral() const
  {
    if (!this->Statements_.empty())
    {
      return std::visit(TokenLiteralVisitor{}, this->Statements_.front());
    }
    else
    {
      return "";
    }
  }

  [[nodiscard]] std::string String() const
  {
    auto ss = std::stringstream{};

    for (auto const& stmt : this->Statements_)
    {
      ss << std::visit(StringVisitor{}, stmt);
    }

    return ss.str();
  }

  void PushStatement(Statement const& statement)
  {
    this->Statements_.push_back(statement);
  }
  std::vector<Statement> Statements_;
};
#endif  // AST_H