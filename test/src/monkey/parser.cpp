//
// Created by wuzhengqi on 24-12-1.
//
#include <gtest/gtest.h>
#include <monkey/parser.h>

#include <memory>

void AssertIdentifiers(std::vector<Identifier> const& lhs, std::vector<Identifier> const& rhs)
{
  ASSERT_EQ(lhs.size(), rhs.size());

  for (auto i = 0u; i < lhs.size(); ++i)
  {
    auto const actualIdentifier = lhs.at(i);
    auto const expectedIdentifier = rhs.at(i);

    ASSERT_EQ(actualIdentifier.Token_, expectedIdentifier.Token_);
    ASSERT_EQ(actualIdentifier.Value_, expectedIdentifier.Value_);
  }
}

TEST(Parser, ParseLetStatement)
{
  std::string_view const input =
      "let x = 5;\
  let y = 10;\
  let foobar = 838383;\
  ";
  auto lexer = make_unique<Lexer>(input);
  auto parser = make_unique<Parser>(std::move(lexer));
  parser->ParseProgram();

  auto program = parser->GetProgram();
  EXPECT_NE(program, nullptr);
  EXPECT_EQ(program->Statements_.size(), 3);

  auto expectedIdentifiers = std::vector{
    Identifier{ .Token_ = Token{ .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = "x" }, .Value_ = "x" },
    Identifier{ .Token_ = Token{ .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = "y" }, .Value_ = "y" },
    Identifier{ .Token_ = Token{ .TokenType_ = ComplicatedTokenType::Ident, .Literal_ = "foobar" }, .Value_ = "foobar" },
  };

  auto actualIdentifiers = std::vector<Identifier>{};
  std::transform(
      program->Statements_.begin(),
      program->Statements_.end(),
      std::back_inserter(actualIdentifiers),
      [](auto const& statement) { return std::get<0>(statement).Name_; });

  AssertIdentifiers(actualIdentifiers, expectedIdentifiers);
}

TEST(Parser, ParseReturnStatement)
{
  std::string_view const input =
      "return 5;\
  return 10;\
  return 838383;\
  ";
  auto lexer = make_unique<Lexer>(input);
  auto parser = make_unique<Parser>(std::move(lexer));
  parser->ParseProgram();

  auto program = parser->GetProgram();
  EXPECT_NE(program, nullptr);
  EXPECT_EQ(program->Statements_.size(), 3);

  auto returnStatements = std::vector<ReturnStatement>{};
  std::transform(
      std::begin(program->Statements_),
      std::end(program->Statements_),
      std::back_inserter(returnStatements),
      [](auto const& statement) { return std::get<1>(statement); });

  auto expectedToken = Token{ .TokenType_ = ComplicatedTokenType::Return, .Literal_ = "return" };

  for (auto const& statement : returnStatements)
  {
    ASSERT_EQ(statement.Token_, expectedToken);
  }
}

TEST(Parser, ParsingIdentifierExpression)
{
  auto const input = "foobar;";
  auto lexer = std::make_unique<Lexer>(input);
  auto parser = std::make_unique<Parser>(std::move(lexer));
  parser->ParseProgram();
  auto program = parser->GetProgram();
  EXPECT_EQ(program->Statements_.size(), 1);

  auto const exprStmt = std::get<2>(program->Statements_.at(0));
  auto const ident = std::get<0>(exprStmt.Expression_);
  EXPECT_EQ(ident.Value_, "foobar");
  EXPECT_EQ(ident.TokenLiteral(), "foobar");
}

TEST(Parser, ParsingIntegerLiteralExpression)
{
  auto const input = "5;";
  auto lexer = std::make_unique<Lexer>(input);
  auto parser = std::make_unique<Parser>(std::move(lexer));
  parser->ParseProgram();
  auto program = parser->GetProgram();
  EXPECT_EQ(program->Statements_.size(), 1);

  auto const intStmt = std::get<2>(program->Statements_.at(0));
  auto const ident = std::get<1>(intStmt.Expression_);
  EXPECT_EQ(ident.Value_, 5);
  EXPECT_EQ(ident.TokenLiteral(), "5");
}

TEST(Parser, ParsingPrefixExpressions)
{
  struct Case
  {
    std::string Input_;
    std::string Operator_;
    int64_t IntegerValue_;
  };

  auto const cases = std::vector<Case>{ Case{
                                            .Input_ = "!5;",
                                            .Operator_ = "!",
                                            .IntegerValue_ = 5,
                                        },
                                        Case{
                                            .Input_ = "-15",
                                            .Operator_ = "-",
                                            .IntegerValue_ = 15,
                                        } };

  for (auto const& c : cases)
  {
    auto lexer = std::make_unique<Lexer>(c.Input_);
    auto parser = std::make_unique<Parser>(std::move(lexer));
    parser->ParseProgram();
    auto program = parser->GetProgram();

    ASSERT_EQ(program->Statements_.size(), 1);
    auto const stmt = std::get<2>(program->Statements_.at(0));
    auto const exp = std::get<2>(stmt.Expression_);
    EXPECT_EQ(exp.Operator_, c.Operator_);
    auto const val = std::get<1>(*exp.Right_);
    EXPECT_EQ(val.Value_, c.IntegerValue_);
  }
}

TEST(Parser, ParsingInfixExpressions)
{
  struct Case
  {
    std::string Input_;
    int64_t LeftValue_;
    std::string Operator_;
    int64_t RightValue_;
  };

  auto const cases = std::vector<Case>{
    Case{
        .Input_ = "5 + 5;",
        .LeftValue_ = 5,
        .Operator_ = "+",
        .RightValue_ = 5,
    },
  };

  for (auto const& c : cases)
  {
    auto lexer = std::make_unique<Lexer>(c.Input_);
    auto parser = std::make_unique<Parser>(std::move(lexer));
    parser->ParseProgram();
    auto program = parser->GetProgram();

    ASSERT_EQ(program->Statements_.size(), 1);
    auto const stmt = std::get<ExpressionStatement>(program->Statements_.at(0));
    auto const exp = std::get<InfixExpression>(stmt.Expression_);
    auto const leftVal = std::get<IntegerLiteral>(*exp.Left_);
    EXPECT_EQ(leftVal.Value_, c.LeftValue_);
    EXPECT_EQ(exp.Operator_, c.Operator_);
    auto const rightVal = std::get<IntegerLiteral>(*exp.Right_);
    EXPECT_EQ(rightVal.Value_, c.RightValue_);
  }
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}