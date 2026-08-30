#include "lexer.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>

void Lexer::TransformTokens(std::string line)
{
    std::string current;

    for (size_t i = 0; i < line.size(); i++)
    {
        auto c = line[i];

        if (c == '"')
        {
            std::string value;

            i++;

            while (i < line.size() && line[i] != '"')
            {
                value += line[i];
                i++;
            }

            tokens.EXPRESSION.push_back({TokenType::String, value});

            continue;
        }

        if (std::isspace(static_cast<unsigned char>(c)))
        {
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            current.clear();

            while (i < line.size() && std::isdigit(static_cast<unsigned char>(line[i])))
            {
                current += line[i];
                i++;
            }

            i--;

            tokens.EXPRESSION.push_back({TokenType::Number, current});

            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c)))
        {
            current.clear();

            while (i < line.size() &&
                   (std::isalnum(static_cast<unsigned char>(line[i])) ||
                    line[i] == '_'))
            {
                current += line[i];
                i++;
            }

            i--;

            if (current == "print")
            {
                tokens.EXPRESSION.push_back(
                    {TokenType::Print, current});
            }
            else if (current == "var")
            {
                tokens.EXPRESSION.push_back(
                    {TokenType::Var, current});
            }
            else if (current == "if")
            {
                tokens.EXPRESSION.push_back(
                    {TokenType::If, current});
            }

            continue;
        }

        if (c == '=')
        {
            if (line[i + 1] == '=')
            {
                tokens.EXPRESSION.push_back({TokenType::Equality, "=="});
            }
            else
            {
                tokens.EXPRESSION.push_back({TokenType::Receives, "="});
            }

            i++;
            continue;
        }

        switch (c)
        {
        case '+':
            tokens.EXPRESSION.push_back({TokenType::Plus, "+"});
            break;

        case '-':
            tokens.EXPRESSION.push_back({TokenType::Minus, "-"});
            break;

        case '*':
            tokens.EXPRESSION.push_back({TokenType::Multiply, "*"});
            break;

        case '/':
            tokens.EXPRESSION.push_back({TokenType::Divide, "/"});
            break;

        case '(':
            tokens.EXPRESSION.push_back({TokenType::LeftParen, "("});
            break;

        case ')':
            tokens.EXPRESSION.push_back({TokenType::RightParen, ")"});
            break;

        case ';':
            tokens.EXPRESSION.push_back({TokenType::Semicolon, ";"});
            break;

        case '"':
            tokens.EXPRESSION.push_back({TokenType::Aspas, "\""});
            break;

        case ':':
            tokens.EXPRESSION.push_back({TokenType::Open, ":"});
            break;

        default:
            throw std::runtime_error("Caractere inválido: " + std::string(1, c));
        }
    }

    tokens.EXPRESSION.push_back({TokenType::EndOfFile, ""});
}
