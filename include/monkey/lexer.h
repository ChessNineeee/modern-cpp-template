//
// Created by wuzhengqi on 24-12-1.
//

#ifndef LEXER_H
#define LEXER_H

#include <format>
#include <memory>

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
template<>
struct std::formatter<SimpleTokenType> : std::formatter<std::string>
{
  auto format(SimpleTokenType t, format_context& ctx) const
  {
    auto to_string_view = [](SimpleTokenType t) -> std::string_view
    {
      #define PROCESS_VAL(p) case(p): return #p;
      switch (t)
      {
        PROCESS_VAL(SimpleTokenType::Plus);
        PROCESS_VAL(SimpleTokenType::Minus);
        PROCESS_VAL(SimpleTokenType::Bang);
        PROCESS_VAL(SimpleTokenType::Asterisk);
        PROCESS_VAL(SimpleTokenType::Slash);
        PROCESS_VAL(SimpleTokenType::Lt);
        PROCESS_VAL(SimpleTokenType::Gt);
        PROCESS_VAL(SimpleTokenType::Assign);
        PROCESS_VAL(SimpleTokenType::Comma);
        PROCESS_VAL(SimpleTokenType::Semicolon);
        PROCESS_VAL(SimpleTokenType::LParen);
        PROCESS_VAL(SimpleTokenType::RParen);
        PROCESS_VAL(SimpleTokenType::LBrace);
        PROCESS_VAL(SimpleTokenType::RBrace);
        PROCESS_VAL(SimpleTokenType::Eof);
      }
      #undef PROCESS_VAL
    };
    return formatter<string>::format(std::format("{}", to_string_view(t)), ctx);
  }
} __attribute__((packed));

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
template<>
struct std::formatter<ComplicatedTokenType> : std::formatter<std::string>
{
  auto format(ComplicatedTokenType t, format_context& ctx) const
  {
    auto to_string = [](ComplicatedTokenType t) -> std::string
    {
      #define PROCESS_VAL(p) case(p): return #p;
      switch (t)
      {
        PROCESS_VAL(ComplicatedTokenType::Ident);
        PROCESS_VAL(ComplicatedTokenType::Int);
        PROCESS_VAL(ComplicatedTokenType::Eq);
        PROCESS_VAL(ComplicatedTokenType::NotEq);
        PROCESS_VAL(ComplicatedTokenType::Function);
        PROCESS_VAL(ComplicatedTokenType::Let);
        PROCESS_VAL(ComplicatedTokenType::True);
        PROCESS_VAL(ComplicatedTokenType::False);
        PROCESS_VAL(ComplicatedTokenType::If);
        PROCESS_VAL(ComplicatedTokenType::Else);
        PROCESS_VAL(ComplicatedTokenType::Return);
      }
      #undef PROCESS_VAL
    };
    return formatter<string>::format(std::format("{}", to_string(t)), ctx);
  }
} __attribute__((packed));

struct Illegal
{
  [[nodiscard]] bool operator==(auto const & _) const
  {
    return true;
  }
};
template<>
struct std::formatter<Illegal> : std::formatter<std::string>
{
  auto format(Illegal _, format_context& ctx) const
  {
    return formatter<string>::format(std::format("ILLEGAL"), ctx);
  }
} __attribute__((packed));


using TokenType = std::variant<SimpleTokenType, ComplicatedTokenType, Illegal>;
template<>
struct std::formatter<TokenType> : std::formatter<std::string>
{
  auto format(TokenType t, format_context& ctx) const
  {
    auto to_string = [](TokenType t) -> std::string
    {
      switch (t.index())
      {
        case 0: return std::format("{}", std::get<0>(t));
        case 1: return std::format("{}", std::get<1>(t));
        case 2: return std::format("{}", std::get<2>(t));
        default: std::terminate();
      }
    };
    return formatter<string>::format(std::format("{}", to_string(t)), ctx);
  }
} __attribute__((packed));

struct TokenTypeHasher
{
  size_t operator()(TokenType t) const
  {
    switch (t.index())
    {
      case 0:
        return static_cast<size_t>(std::get<0>(t));
      case 1:
        return 1000 + static_cast<size_t>(std::get<1>(t));
      case 2:
        return 2000;
      default:
        std::terminate();
    }
  }
};


struct Token
{
  [[nodiscard]] bool Is(SimpleTokenType t) const
  {
    if (this->TokenType_.index() != 0) return false;
    auto SimpleTokenTypeEquals = std::equal_to<SimpleTokenType>{};
    return SimpleTokenTypeEquals(std::get<0>(this->TokenType_), t);
  }

  [[nodiscard]] bool Is(ComplicatedTokenType t) const
  {
    if (this->TokenType_.index() != 1) return false;
    auto SimpleTokenTypeEquals = std::equal_to<ComplicatedTokenType>{};
    return SimpleTokenTypeEquals(std::get<1>(this->TokenType_), t);
  }

  bool operator==(const Token& rhs) const
  {
    return this->TokenType_ == rhs.TokenType_ && this->Literal_ == rhs.Literal_;
  }

  TokenType TokenType_;
  std::string Literal_;
} __attribute__((aligned(64)));

template<>
struct std::formatter<Token> : std::formatter<std::string>
{
  auto format(Token t, format_context& ctx) const
  {
    return formatter<string>::format(std::format("[Type: {} | Literal: {}]", t.TokenType_, t.Literal_), ctx);
  }
} __attribute__((packed));


class Lexer
{
public:
  explicit Lexer(std::string_view input);
  ~Lexer();
  [[nodiscard]] Token PeekBackToken() const;
  void ParseToken();

private:
  struct LexerImpl;
  std::unique_ptr<LexerImpl> LexerImpl_;
};
#endif //LEXER_H
