//
// Created by wuzhengqi on 24-12-1.
//

#include <monkey/lexer.h>

#include <vector>
#include <algorithm>
#include <format>
#include <memory>
[[nodiscard]] static Token NewToken(TokenType t, std::string_view literal)
{
  return Token {
    .TokenType_ = t,
    .Literal_ = std::string(literal),
  };
}

struct Lexer::LexerImpl
{
  explicit LexerImpl(std::string_view input) : Input_(input) {}
  std::string Input_;
  int32_t Position_ = 0;
  int32_t ReadPosition_ = 0;
  char CurrentChar_ = 0;
  std::vector<Token> Tokens_;
  std::tuple<size_t, size_t> Token_Interval_ = std::make_tuple(0, 0);
  static constexpr std::array SingleCharTokenSet = {'=', ';', '(', ')', '{', '}', ',', '+', '-', '!', '*', '/', '<', '>', '\0'};

  void UpdateStatusBySingleCharToken();
  void ParseNumberOrIdentifierOrEqualsToken();
  void ParseSingleCharToken();
  void PrepareBeforeParse();
  void SkipWhitespace();
  [[nodiscard]] bool HasSingleCharToken() const;
  [[nodiscard]] Token GetSingleCharToken() const;
  [[nodiscard]] bool HasNumberOrIdentifierOrEqualsToken() const;
  void UpdateStatusByNumberOrIdentifierOrEqualsToken();
  [[nodiscard]] Token GetNumberOrIdentifierOrEqualsTokenByStartChAndInterval(char) const;
  void UpdateStatusByNumberToken();
  void UpdateStatusByIdentifierToken();
  void UpdateStatusByEqualsToken();
  [[nodiscard]] char PeekCurrentChar() const;
  [[nodiscard]] char PeekNextChar() const;
  void UpdateChByReadPosition();
  void UpdatePositionByReadPosition();
  void IncrementReadPosition();
};

Lexer::Lexer(std::string_view input)
{
    this->LexerImpl_ = std::make_unique<LexerImpl>(input);
    this->LexerImpl_->UpdateStatusBySingleCharToken();
}

Lexer::~Lexer() = default;

[[nodiscard]] Token Lexer::PeekBackToken() const
{
  return *std::rbegin(this->LexerImpl_->Tokens_);
}

void Lexer::ParseToken()
{
  this->LexerImpl_->PrepareBeforeParse();
  if (this->LexerImpl_->HasNumberOrIdentifierOrEqualsToken())
  {
    this->LexerImpl_->ParseNumberOrIdentifierOrEqualsToken();
  } else if (this->LexerImpl_->HasSingleCharToken()) {
    this->LexerImpl_->ParseSingleCharToken();
  } else
  {
    std::terminate();
  }
}

void Lexer::LexerImpl::ParseNumberOrIdentifierOrEqualsToken()
{
  auto tokenStartCh = this->PeekCurrentChar();
  this->UpdateStatusByNumberOrIdentifierOrEqualsToken();
  auto token = this->GetNumberOrIdentifierOrEqualsTokenByStartChAndInterval(tokenStartCh);
  this->Tokens_.emplace_back(token);
}

void Lexer::LexerImpl::ParseSingleCharToken()
{
  auto token = this->GetSingleCharToken();
  this->UpdateStatusBySingleCharToken();
  this->Tokens_.emplace_back(token);
}



void Lexer::LexerImpl::PrepareBeforeParse()
{
  this->SkipWhitespace();
}

void Lexer::LexerImpl::SkipWhitespace()
{
  while (std::isspace(this->CurrentChar_) != 0)
  {
    this->UpdateStatusBySingleCharToken();
  }
}

[[nodiscard]] bool Lexer::LexerImpl::HasSingleCharToken() const
{
  return std::find(std::begin(Lexer::LexerImpl::SingleCharTokenSet), std::end(Lexer::LexerImpl::SingleCharTokenSet), this->CurrentChar_) != std::end(Lexer::LexerImpl::SingleCharTokenSet);
}

[[nodiscard]] Token Lexer::LexerImpl::GetSingleCharToken() const
{
  switch (this->CurrentChar_)
  {
    case '=': return NewToken(SimpleTokenType::Assign, "=");
    case ',': return NewToken(SimpleTokenType::Comma, ",");
    case ';': return NewToken(SimpleTokenType::Semicolon, ";");
    case '(': return NewToken(SimpleTokenType::LParen, "(");
    case ')': return NewToken(SimpleTokenType::RParen, ")");
    case '{': return NewToken(SimpleTokenType::LBrace, "{");
    case '}': return NewToken(SimpleTokenType::RBrace, "}");
    case '+': return NewToken(SimpleTokenType::Plus, "+");
    case '-': return NewToken(SimpleTokenType::Minus, "-");
    case '!': return NewToken(SimpleTokenType::Bang, "!");
    case '*': return NewToken(SimpleTokenType::Asterisk, "*");
    case '/': return NewToken(SimpleTokenType::Slash, "/");
    case '<': return NewToken(SimpleTokenType::Lt, "<");
    case '>': return NewToken(SimpleTokenType::Gt, ">");
    case '\0': return NewToken(SimpleTokenType::Eof, "");
    default: std::terminate();
  }
}

[[nodiscard]] bool Lexer::LexerImpl::HasNumberOrIdentifierOrEqualsToken() const
{
  if (std::isdigit(static_cast<unsigned char>(this->CurrentChar_)) != 0)
  {
    return true;
  }

  if (std::isalpha(static_cast<unsigned char>(this->CurrentChar_)) != 0)
  {
    return true;
  }

  if ((this->CurrentChar_ == '=' || this->CurrentChar_ == '!') && this->PeekNextChar() == '=')
  {
    return true;
  }

  return false;
}

void Lexer::LexerImpl::UpdateStatusByNumberOrIdentifierOrEqualsToken()
{
  if (std::isdigit(static_cast<unsigned char>(this->CurrentChar_)) != 0)
  {
    this->UpdateStatusByNumberToken(); return;
  }

  if (std::isalpha(static_cast<unsigned char>(this->CurrentChar_)) != 0)
  {
    this->UpdateStatusByIdentifierToken(); return;
  }

  if ((this->CurrentChar_ == '=' || this->CurrentChar_ == '!') && this->PeekNextChar() == '=')
  {
    this->UpdateStatusByEqualsToken(); return;
  }
}

[[nodiscard]] static TokenType IdentifierLiteralToTokenType(std::string_view literal);

[[nodiscard]] Token Lexer::LexerImpl::GetNumberOrIdentifierOrEqualsTokenByStartChAndInterval(char startCh) const
{
  auto const literal = this->Input_.substr(std::get<0>(this->Token_Interval_), std::get<1>(this->Token_Interval_) - std::get<0>(this->Token_Interval_));
  if (std::isdigit(static_cast<unsigned char>(startCh)) != 0)
  {
    return NewToken(ComplicatedTokenType::Int, literal);
  }

  if (std::isalpha(static_cast<unsigned char>(startCh)) != 0)
  {
    return NewToken(IdentifierLiteralToTokenType(literal), literal);
  }

  if (startCh == '=')
  {
    return NewToken(ComplicatedTokenType::Eq, literal);
  }

  if (startCh == '!')
  {
    return NewToken(ComplicatedTokenType::NotEq, literal);
  }

  std::terminate();
}

void Lexer::LexerImpl::UpdateStatusByNumberToken()
{
  auto leftClosed = this->Position_;
  while (std::isdigit(static_cast<unsigned char>(this->CurrentChar_)) != 0)
  {
    this->UpdateStatusBySingleCharToken();
  }
  this->Token_Interval_ = std::make_tuple(leftClosed, this->Position_);
}

void Lexer::LexerImpl::UpdateStatusByIdentifierToken()
{
  auto leftClosed = this->Position_;
  while (std::isalpha(static_cast<unsigned char>(this->CurrentChar_)) != 0)
  {
    this->UpdateStatusBySingleCharToken();
  }
  this->Token_Interval_ = std::make_tuple(leftClosed, this->Position_);
}

void Lexer::LexerImpl::UpdateStatusByEqualsToken()
{
  auto leftClosed = this->Position_;
  auto fixedTokenIntervalLength = 2;
  for (auto i = 0; i < fixedTokenIntervalLength; i++)
  {
    this->UpdateStatusBySingleCharToken();
  }
  this->Token_Interval_ = std::make_tuple(leftClosed, this->Position_);
}

[[nodiscard]] char Lexer::LexerImpl::PeekCurrentChar() const
{
  if (static_cast<size_t>(this->Position_) >= this->Input_.size())
  {
    return '\0';
  }

  return this->Input_.at(static_cast<size_t>(this->Position_));
}

[[nodiscard]] char Lexer::LexerImpl::PeekNextChar() const
{
  if (static_cast<size_t>(this->ReadPosition_) >= this->Input_.size())
  {
    return '\0';
  }

  return this->Input_.at(static_cast<size_t>(this->ReadPosition_));
}

void Lexer::LexerImpl::UpdateStatusBySingleCharToken()
{
  this->UpdateChByReadPosition();
  this->UpdatePositionByReadPosition();
  this->IncrementReadPosition();
}

void Lexer::LexerImpl::UpdateChByReadPosition()
{
  if (static_cast<size_t>(this->ReadPosition_) >= this->Input_.size())
  {
    this->CurrentChar_ = '\0';
  } else
  {
    this->CurrentChar_ = this->Input_.at(static_cast<size_t>(this->ReadPosition_));
  }
}

void Lexer::LexerImpl::UpdatePositionByReadPosition()
{
   this->Position_ = this->ReadPosition_;
}

void Lexer::LexerImpl::IncrementReadPosition()
{
  this->ReadPosition_ += 1;
}

[[nodiscard]] static TokenType IdentifierLiteralToTokenType(std::string_view literal)
{
  if (literal == "let")
  {
    return ComplicatedTokenType::Let;
  }

  if (literal == "true")
  {
    return ComplicatedTokenType::True;
  }

  if (literal == "false")
  {
    return ComplicatedTokenType::False;
  }

  if (literal == "if")
  {
    return ComplicatedTokenType::If;
  }

  if (literal == "else")
  {
    return ComplicatedTokenType::Else;
  }

  if (literal == "return")
  {
    return ComplicatedTokenType::Return;
  }

  if (literal == "fn")
  {
    return ComplicatedTokenType::Function;
  }

  return ComplicatedTokenType::Ident;
}