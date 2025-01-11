//
// Created by wuzhengqi on 24-12-22.
//

#include "monkey/ast.h"

#include <gtest/gtest.h>

TEST(AST, String)
{
  auto program = Program{};
  auto letStatement = LetStatement{
    .Token_ = Token{
      .TokenType_ = ComplicatedTokenType::Let,
      .Literal_ = "let",
    },
    .Name_ = Identifier{
      .Token_ = Token{
        .TokenType_ = ComplicatedTokenType::Ident,
        .Literal_ = "myVar",
      },
      .Value_ = "myVar",
    },
    .Value_ = Identifier{
      .Token_ = Token{
        .TokenType_ = ComplicatedTokenType::Ident,
        .Literal_ = "anotherVar",
      },
      .Value_ = "anotherVar",
    },
  };
  program.PushStatement(letStatement);
  ASSERT_EQ(program.String(), "let myVar = anotherVar;");
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}