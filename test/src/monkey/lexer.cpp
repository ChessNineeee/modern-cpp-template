//
// Created by wuzhengqi on 24-11-30.
//
#include <algorithm>
#include <gtest/gtest.h>
enum class SimpleTokenType
{
  Plus,
  Minus,
  Bang,
  Asterisk,
  Slash,
  Lt,
  Gt,
  Assign,
  Comma,
  Semicolon,
  LParen,
  RParen,
  LBrace,
  RBrace,
  Eof,
};

enum class ComplicatedTokenType
{
  Ident,
  Int,
  Eq,
  NotEq,
  Function,
  Let,
  True,
  False,
  If,
  Else,
  Return,
};

struct Illegal
{
  [[nodiscard]] bool operator==(auto const & _) const
  {
    return true;
  }
};

using TokenType = std::variant<SimpleTokenType, ComplicatedTokenType, Illegal>;

[[nodiscard]] static TokenType IdentifierLiteralToTokenType(std::string_view literal);

struct Token
{
  TokenType TokenType_;
  std::string Literal_;
} __attribute__((aligned(64)));

[[nodiscard]] static Token NewToken(TokenType t, std::string_view literal)
{
  return Token {
    .TokenType_ = t,
    .Literal_ = std::string(literal),
  };
}

class  Lexer
{
public:
  explicit Lexer(std::string_view input) : Input_(input)
  {
    UpdateStatusBySingleCharToken();
  }
  void UpdateStatusBySingleCharToken();
  void ParseToken();
  void ParseNumberOrIdentifierOrEqualsToken();
  void ParseSingleCharToken();
  [[nodiscard]] Token PeekBackToken() const;
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
private:
  std::string Input_;
  int32_t Position_ = 0;
  int32_t ReadPosition_ = 0;
  char CurrentChar_ = 0;
  std::vector<Token> Tokens_;
  std::tuple<size_t, size_t> Token_Interval_ = std::make_tuple(0, 0);
  static constexpr std::array SingleCharTokenSet = {'=', ';', '(', ')', '{', '}', ',', '+', '-', '!', '*', '/', '<', '>', '\0'};
};

void Lexer::ParseToken()
{
  this->PrepareBeforeParse();
  if (this->HasNumberOrIdentifierOrEqualsToken())
  {
    this->ParseNumberOrIdentifierOrEqualsToken();
  } else if (this->HasSingleCharToken()) {
    this->ParseSingleCharToken();
  } else
  {
    std::terminate();
  }
}

void Lexer::ParseNumberOrIdentifierOrEqualsToken()
{
  auto tokenStartCh = this->PeekCurrentChar();
  this->UpdateStatusByNumberOrIdentifierOrEqualsToken();
  auto token = this->GetNumberOrIdentifierOrEqualsTokenByStartChAndInterval(tokenStartCh);
  this->Tokens_.emplace_back(token);
}

void Lexer::ParseSingleCharToken()
{
  auto token = this->GetSingleCharToken();
  this->UpdateStatusBySingleCharToken();
  this->Tokens_.emplace_back(token);
}

[[nodiscard]] Token Lexer::PeekBackToken() const
{
  return *std::rbegin(this->Tokens_);
}

void Lexer::PrepareBeforeParse()
{
  this->SkipWhitespace();
}

void Lexer::SkipWhitespace()
{
  while (std::isspace(this->CurrentChar_))
  {
    this->UpdateStatusBySingleCharToken();
  }
}

[[nodiscard]] bool Lexer::HasSingleCharToken() const
{
  return std::find(std::begin(Lexer::SingleCharTokenSet), std::end(Lexer::SingleCharTokenSet), this->CurrentChar_) != std::end(Lexer::SingleCharTokenSet);
}

[[nodiscard]] Token Lexer::GetSingleCharToken() const
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

[[nodiscard]] bool Lexer::HasNumberOrIdentifierOrEqualsToken() const
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

void Lexer::UpdateStatusByNumberOrIdentifierOrEqualsToken()
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

[[nodiscard]] Token Lexer::GetNumberOrIdentifierOrEqualsTokenByStartChAndInterval(char startCh) const
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

void Lexer::UpdateStatusByNumberToken()
{
  auto leftClosed = this->Position_;
  while (std::isdigit(static_cast<unsigned char>(this->CurrentChar_)) != 0)
  {
    this->UpdateStatusBySingleCharToken();
  }
  this->Token_Interval_ = std::make_tuple(leftClosed, this->Position_);
}

void Lexer::UpdateStatusByIdentifierToken()
{
  auto leftClosed = this->Position_;
  while (std::isalpha(static_cast<unsigned char>(this->CurrentChar_)) != 0)
  {
    this->UpdateStatusBySingleCharToken();
  }
  this->Token_Interval_ = std::make_tuple(leftClosed, this->Position_);
}

void Lexer::UpdateStatusByEqualsToken()
{
  auto leftClosed = this->Position_;
  auto fixedTokenIntervalLength = 2;
  for (auto i = 0; i < fixedTokenIntervalLength; i++)
  {
    this->UpdateStatusBySingleCharToken();
  }
  this->Token_Interval_ = std::make_tuple(leftClosed, this->Position_);
}

[[nodiscard]] char Lexer::PeekCurrentChar() const
{
  if (static_cast<size_t>(this->Position_) >= this->Input_.size())
  {
    return '\0';
  }

  return this->Input_.at(static_cast<size_t>(this->Position_));
}

[[nodiscard]] char Lexer::PeekNextChar() const
{
  if (static_cast<size_t>(this->ReadPosition_) >= this->Input_.size())
  {
    return '\0';
  }

  return this->Input_.at(static_cast<size_t>(this->ReadPosition_));
}

void Lexer::UpdateStatusBySingleCharToken()
{
  this->UpdateChByReadPosition();
  this->UpdatePositionByReadPosition();
  this->IncrementReadPosition();
}

void Lexer::UpdateChByReadPosition()
{
  if (static_cast<size_t>(this->ReadPosition_) >= this->Input_.size())
  {
    this->CurrentChar_ = '\0';
  } else
  {
    this->CurrentChar_ = this->Input_.at(static_cast<size_t>(this->ReadPosition_));
  }
}

void Lexer::UpdatePositionByReadPosition()
{
   this->Position_ = this->ReadPosition_;
}

void Lexer::IncrementReadPosition()
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

TEST(Lexer, NextToken)
{
  std::string_view const input = "let five = 5;\
          let ten = 10;\
    let add = fn(x, y) {\
      x + y;\
    };\
    let result = add(five, ten);\
    !-/*5;\
    5 < 10 > 5;\
    10 == 10;\
    10 != 9;";
  struct ExpectedResult
  {
    TokenType TokenType_;
    std::string Literal_;
  } __attribute__((aligned(64)));

  auto results = std::vector{
    ExpectedResult{
      .TokenType_ = ComplicatedTokenType::Let,
      .Literal_ = std::string("let"),
    },
    ExpectedResult{
      .TokenType_ = ComplicatedTokenType::Ident,
      .Literal_ = std::string("five"),
    },
    { .TokenType_ = SimpleTokenType::Assign, .Literal_ = std::string("=") },
    { .TokenType_ = ComplicatedTokenType::Int, .Literal_ = std::string("5") },
    { .TokenType_ = SimpleTokenType::Semicolon, .Literal_ = std::string(";") },
    { .TokenType_ = ComplicatedTokenType::Let, .Literal_ = std::string("let") },
    { .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = std::string("ten") },
    { .TokenType_ = SimpleTokenType::Assign, .Literal_ = std::string("=") },
    { .TokenType_ = ComplicatedTokenType::Int, .Literal_ = std::string("10") },
    { .TokenType_ = SimpleTokenType::Semicolon, .Literal_ = std::string(";") },
    { .TokenType_ = ComplicatedTokenType::Let, .Literal_ = std::string("let") },
    { .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = std::string("add") },
    { .TokenType_ = SimpleTokenType::Assign, .Literal_ = std::string("=") },
    { .TokenType_ = ComplicatedTokenType::Function, .Literal_ = std::string("fn") },
    { .TokenType_ = SimpleTokenType::LParen, .Literal_ = std::string("(") },
    { .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = std::string("x") },
    { .TokenType_ = SimpleTokenType::Comma, .Literal_ = std::string(",") },
    { .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = std::string("y") },
    { .TokenType_ = SimpleTokenType::RParen, .Literal_ = std::string(")") },
    { .TokenType_ = SimpleTokenType::LBrace, .Literal_ = std::string("{") },
    { .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = std::string("x") },
    { .TokenType_ = SimpleTokenType::Plus, .Literal_ = std::string("+") },
    { .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = std::string("y") },
    { .TokenType_ = SimpleTokenType::Semicolon, .Literal_ = std::string(";") },
    { .TokenType_ = SimpleTokenType::RBrace, .Literal_ = std::string("}") },
    { .TokenType_ = SimpleTokenType::Semicolon, .Literal_ = std::string(";") },
    { .TokenType_ = ComplicatedTokenType::Let, .Literal_ = std::string("let") },
    { .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = std::string("result") },
    { .TokenType_ = SimpleTokenType::Assign, .Literal_ = std::string("=") },
    { .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = std::string("add") },
    { .TokenType_ = SimpleTokenType::LParen, .Literal_ = std::string("(") },
    { .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = std::string("five") },
    { .TokenType_ = SimpleTokenType::Comma, .Literal_ = std::string(",") },
    { .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = std::string("ten") },
    { .TokenType_ = SimpleTokenType::RParen, .Literal_ = std::string(")") },
    { .TokenType_ = SimpleTokenType::Semicolon, .Literal_ = std::string(";") },
    { .TokenType_ = SimpleTokenType::Bang, .Literal_ = std::string("!") },
    { .TokenType_ = SimpleTokenType::Minus, .Literal_ = std::string("-") },
    { .TokenType_ = SimpleTokenType::Slash, .Literal_ = std::string("/") },
    { .TokenType_ = SimpleTokenType::Asterisk, .Literal_ = std::string("*") },
    { .TokenType_ = ComplicatedTokenType::Int, .Literal_ = std::string("5") },
    { .TokenType_ = SimpleTokenType::Semicolon, .Literal_ = std::string(";") },
    { .TokenType_ = ComplicatedTokenType::Int, .Literal_ = std::string("5") },
    { .TokenType_ = SimpleTokenType::Lt, .Literal_ = std::string("<") },
    { .TokenType_ = ComplicatedTokenType::Int, .Literal_ = std::string("10") },
    { .TokenType_ = SimpleTokenType::Gt, .Literal_ = std::string(">") },
    { .TokenType_ = ComplicatedTokenType::Int, .Literal_ = std::string("5") },
    { .TokenType_ = SimpleTokenType::Semicolon, .Literal_ = std::string(";") },
    { .TokenType_ = ComplicatedTokenType::Int, .Literal_ = std::string("10") },
    { .TokenType_ = ComplicatedTokenType::Eq, .Literal_ = std::string("==") },
    { .TokenType_ = ComplicatedTokenType::Int, .Literal_ = std::string("10") },
    { .TokenType_ = SimpleTokenType::Semicolon, .Literal_ = std::string(";") },
    { .TokenType_ = ComplicatedTokenType::Int, .Literal_ = std::string("10") },
    { .TokenType_ = ComplicatedTokenType::NotEq, .Literal_ = std::string("!=") },
    { .TokenType_ = ComplicatedTokenType::Int, .Literal_ = std::string("9") },
    { .TokenType_ = SimpleTokenType::Semicolon, .Literal_ = std::string(";") },
    { .TokenType_ = SimpleTokenType::Eof, .Literal_ = std::string("") },
    { .TokenType_ = SimpleTokenType::Eof, .Literal_ = std::string("") },
    { .TokenType_ = SimpleTokenType::Eof, .Literal_ = std::string("") },
  };

  auto lexer = Lexer(input);
  for (auto const & result : results)
  {
    lexer.ParseToken();
    auto const token = lexer.PeekBackToken();
    ASSERT_EQ(result.TokenType_,  token.TokenType_);
    ASSERT_EQ(result.Literal_, token.Literal_);
  }
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}