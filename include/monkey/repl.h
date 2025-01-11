//
// Created by wuzhengqi on 24-12-1.
//

#ifndef REPL_H
#define REPL_H
// Read Eval Print Loop
class Repl
{
public:
  void Start();
private:
  void DisplayPrompt();
  [[nodiscard]] auto GetLine();
  void DisplayTokens(std::string_view line);
  void DisplayToken(auto token);
  constexpr static auto prompt = ">> ";
  std::istream& in { std::cin };
  std::ostream& out { std::cout };
};

void Repl::DisplayPrompt()
{
  this->out << this->prompt;
}


[[nodiscard]] auto Repl::GetLine()
{
  auto line = std::string{};
  std::getline(this->in, line);
  return line;
}

void Repl::Start()
{
  this->DisplayPrompt();
  auto line = GetLine();
  this->DisplayTokens(line);
}

void Repl::DisplayTokens(std::string_view line)
{
  auto lexer = Lexer{line};
  lexer.ParseToken();
  for (auto token = lexer.PeekBackToken(); !token.IsEof(); lexer.ParseToken(), token = lexer.PeekBackToken())
  {
    this->DisplayToken(token);
  }
}

void Repl::DisplayToken(auto token)
{
  this->out << std::format("Token: {}\n", token);
}
#endif //REPL_H
