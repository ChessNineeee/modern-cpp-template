//
// Created by wuzhengqi on 24-11-30.
//
#include <gtest/gtest.h>
#include <monkey/lexer.h>

TEST(Token, IsEof)
{
  auto token = Token{.TokenType_=SimpleTokenType::Eof, .Literal_=""};
  EXPECT_TRUE(token.Is(SimpleTokenType::Eof));

  auto token3 = Token{.TokenType_=SimpleTokenType::Assign, .Literal_="="};
  EXPECT_FALSE(token3.Is(SimpleTokenType::Eof));

  auto token4 = Token{.TokenType_=ComplicatedTokenType::Else, .Literal_="else"};
  EXPECT_FALSE(token4.Is(SimpleTokenType::Eof));

  auto token5 = Token{.TokenType_=Illegal{}, .Literal_=""};
  EXPECT_FALSE(token5.Is(SimpleTokenType::Eof));
}

TEST(Token, IsLet)
{
  auto token = Token{.TokenType_=ComplicatedTokenType::Let, .Literal_="let"};
  EXPECT_TRUE(token.Is(ComplicatedTokenType::Let));

  auto token3 = Token{.TokenType_=SimpleTokenType::Assign, .Literal_="="};
  EXPECT_FALSE(token3.Is(ComplicatedTokenType::Let));

  auto token4 = Token{.TokenType_=ComplicatedTokenType::Else, .Literal_="else"};
  EXPECT_FALSE(token4.Is(ComplicatedTokenType::Let));

  auto token5 = Token{.TokenType_=Illegal{}, .Literal_=""};
  EXPECT_FALSE(token5.Is(ComplicatedTokenType::Let));
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