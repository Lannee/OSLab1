#pragma once

#include "pipe.hh"

#include <memory>
#include <unistd.h>
#include <vector>
#include <cassert>

namespace oslab1::shell::token {

class Token {
public:
    enum Kind {
        Ident,
        Pipe,
        Exit
    };

    Token(Kind kind) : kind_(kind) {}

    auto getKind() const -> Kind { return kind_; }

private:
    Kind kind_;
};

class IdentToken : public Token {
public:
    enum Type {
        Cmd,
        Arg
    };

    IdentToken(std::string_view ident, Type type) 
        : Token(Kind::Ident), ident_(ident), type_(type) {}

    auto getIdent() -> std::string_view { return ident_; }
    auto getType() { return type_; }

private:
    std::string ident_;
    Type type_;
};

class PipeToken : public Token {
public:
    PipeToken() 
        : Token(Kind::Pipe), pipe_(new oslab1::shell::Pipe()) {}

    auto get_pipe() { return pipe_; }

private:
    std::shared_ptr<oslab1::shell::Pipe> pipe_;
};

auto get_tokens(std::string_view line) -> std::vector<std::unique_ptr<token::Token>>;

} // oslab1::shell::token
