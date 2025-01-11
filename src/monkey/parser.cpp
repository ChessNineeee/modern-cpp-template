//
// Created by wuzhengqi on 24-12-1.
//
#include <monkey/lexer.h>
#include <monkey/parser.h>

#include <memory>
#include <unordered_map>

[[nodiscard]] PrefixExpressionNode ConvFrom(Expression const& e)
{
  switch (e.index())
  {
    case 0: return std::get<0>(e);
    case 1: return std::get<1>(e);
    case 2: return std::get<2>(e);
    default: std::terminate();
  }
}

struct Parser::ParserImpl : std::enable_shared_from_this<Parser::ParserImpl>
{
  explicit ParserImpl(std::unique_ptr<Lexer> lexer) : Lexer_(std::move(lexer))
  {
    this->NextToken();
    this->NextToken();
  }

  void ParseProgram();
  [[nodiscard]] std::shared_ptr<Program> GetProgram() const;
  void NextToken();
  void UpdateCurToken();
  void UpdatePeekToken();
  void UpdateProgramByParseStatement();
  void ParseStatement();
  void ParseLetStatement();
  void ParseLetStatementName();
  void ParseReturnStatement();
  void ParseExpressionOfStatement();
  void ParseExpressionStatement();

  std::optional<Expression> GetExpressionByParsingFunction(Precedence);
  std::optional<Expression> GetParsedPrefix();
  Expression GetExpressionByCurToken();
  std::optional<IntegerLiteral> GetIntegerLiteralByCurToken();
  void ParsePrefixExpressionByCurToken();

  template<typename T>
  [[nodiscard]] bool ExpectPeekTokenIs(T t) const
  {
    return this->PeekToken_.Is(t);
  }
  template<typename T>
  [[nodiscard]] bool ExpectCurTokenIs(T t) const
  {
    return this->CurToken_.Is(t);
  }

  [[nodiscard]] bool ExpectCurTokenIsPrefix() const
  {
    if (this->ExpectCurTokenIs(SimpleTokenType::Bang))
    {
      return true;
    }

    if (this->ExpectCurTokenIs(SimpleTokenType::Minus))
    {
      return true;
    }

    return false;
  }

  std::unique_ptr<Lexer> Lexer_;
  Token CurToken_;
  Token PeekToken_;

  std::shared_ptr<Program> Program_ = std::make_shared<Program>();
  std::optional<Statement> Statement_ = std::nullopt;
  std::optional<Identifier> ParsedStatementName = std::nullopt;
  std::optional<Expression> ParsedStatementExpression = std::nullopt;
  std::optional<PrefixExpression> ParsedPrefixExpression = std::nullopt;
};

Parser::Parser(std::unique_ptr<Lexer> lexer)
{
  this->ParserImpl_ = std::make_unique<ParserImpl>(std::move(lexer));
}

void Parser::ParseProgram()
{
  this->ParserImpl_->ParseProgram();
}

[[nodiscard]] std::shared_ptr<Program> Parser::GetProgram() const
{
  return this->ParserImpl_->GetProgram();
}

Parser::~Parser()
{
}

void Parser::ParserImpl::ParseProgram()
{
  while (!this->CurToken_.Is(SimpleTokenType::Eof))
  {
    this->UpdateProgramByParseStatement();
    this->NextToken();
  }
}

[[nodiscard]] std::shared_ptr<Program> Parser::ParserImpl::GetProgram() const
{
  return this->Program_;
}

void Parser::ParserImpl::NextToken()
{
  this->UpdateCurToken();
  this->UpdatePeekToken();
}

void Parser::ParserImpl::UpdateCurToken()
{
  this->CurToken_ = this->PeekToken_;
}

void Parser::ParserImpl::UpdatePeekToken()
{
  this->Lexer_->ParseToken();
  this->PeekToken_ = this->Lexer_->PeekBackToken();
}

void Parser::ParserImpl::UpdateProgramByParseStatement()
{
  this->ParseStatement();
  if (this->Statement_)
  {
    this->Program_->PushStatement(*this->Statement_);
  }
}

void Parser::ParserImpl::ParseStatement()
{
  if (this->CurToken_.Is(ComplicatedTokenType::Let))
  {
    this->ParseLetStatement();
    return;
  }

  if (this->CurToken_.Is(ComplicatedTokenType::Return))
  {
    this->ParseReturnStatement();
    return;
  }

  this->ParseExpressionStatement();
}

void Parser::ParserImpl::ParseLetStatement()
{
  auto stmt = LetStatement{ .Token_ = this->CurToken_ };

  this->ParseLetStatementName();
  if (!this->ParsedStatementName)
  {
    this->Statement_ = std::nullopt;
    return;
  }

  stmt.Name_ = this->ParsedStatementName.value();

  if (!this->ExpectPeekTokenIs(SimpleTokenType::Assign))
  {
    this->Statement_ = std::nullopt;
    return;
  }
  this->NextToken();

  this->ParseExpressionOfStatement();
  if (!this->ParsedStatementExpression)
  {
    this->Statement_ = std::nullopt;
    return;
  }

  stmt.Value_ = this->ParsedStatementExpression.value();

  this->Statement_ = stmt;
}

void Parser::ParserImpl::ParseReturnStatement()
{
  auto stmt = ReturnStatement{ .Token_ = this->CurToken_ };

  this->ParseExpressionOfStatement();
  if (!this->ParsedStatementExpression)
  {
    this->Statement_ = std::nullopt;
    return;
  }

  stmt.ReturnValue_ = this->ParsedStatementExpression.value();

  this->Statement_ = stmt;
}

void Parser::ParserImpl::ParseLetStatementName()
{
  if (!this->ExpectPeekTokenIs(ComplicatedTokenType::Ident))
  {
    this->ParsedStatementName = std::nullopt;
    return;
  }

  this->NextToken();
  this->ParsedStatementName = Identifier{ .Token_ = this->CurToken_, .Value_ = this->CurToken_.Literal_ };
}

void Parser::ParserImpl::ParseExpressionOfStatement()
{
  while (!this->ExpectCurTokenIs(SimpleTokenType::Semicolon))
  {
    this->NextToken();
  }
  // TODO Set parsed statement expression to a meaningful value
  this->ParsedStatementExpression = Identifier{ .Token_ = this->CurToken_, .Value_ = this->CurToken_.Literal_ };
}

void Parser::ParserImpl::ParseExpressionStatement()
{
  auto stmt = ExpressionStatement{ .Token_ = this->CurToken_ };

  auto exprOp = this->GetExpressionByParsingFunction(Precedence::LOWEST);
  if (!exprOp.has_value())
  {
    return;
  }
  stmt.Expression_ = exprOp.value();

  if (!this->ExpectCurTokenIs(SimpleTokenType::Semicolon))
  {
    this->NextToken();
  }

  this->Statement_ = stmt;
}

std::optional<int64_t> StringToInt64(std::string const& literal)
{
  try
  {
    auto result = std::stol(literal);
    return static_cast<int64_t>(result);
  }
  catch (std::exception e)
  {
    return std::nullopt;
  }
}

std::optional<IntegerLiteral> Parser::ParserImpl::GetIntegerLiteralByCurToken()
{
  auto literal = IntegerLiteral{
    .Token_ = this->CurToken_,
  };

  auto val = StringToInt64(this->CurToken_.Literal_);
  if (!val.has_value())
  {
    return std::nullopt;
  }

  literal.Value_ = val.value();
  return literal;
}

void Parser::ParserImpl::ParsePrefixExpressionByCurToken()
{
  auto exp = PrefixExpression{
    .Token_ = this->CurToken_,
    .Operator_ = this->CurToken_.Literal_,
  };

  this->NextToken();

  auto rightOp = this->GetExpressionByParsingFunction(Precedence::PREFIX);
  if (!rightOp.has_value())
  {
    return;
  }

  auto rightExpressionNode = ConvFrom(rightOp.value());

  exp.Right_ = std::make_shared<PrefixExpressionNode>(rightExpressionNode);

  this->ParsedPrefixExpression = exp;
}

std::optional<Expression> Parser::ParserImpl::GetExpressionByParsingFunction(Precedence _)
{
  return this->GetParsedPrefix();
}

std::optional<Expression> Parser::ParserImpl::GetParsedPrefix()
{
  if (this->ExpectCurTokenIs(ComplicatedTokenType::Ident))
  {
    return this->GetExpressionByCurToken();
  }

  if (this->ExpectCurTokenIs(ComplicatedTokenType::Int))
  {
    return this->GetIntegerLiteralByCurToken();
  }

  if (this->ExpectCurTokenIsPrefix())
  {
    this->ParsePrefixExpressionByCurToken();
    return this->ParsedPrefixExpression;
  }

  return std::nullopt;
}

Expression Parser::ParserImpl::GetExpressionByCurToken()
{
  return Identifier{ .Token_ = this->CurToken_, .Value_ = this->CurToken_.Literal_ };
}
