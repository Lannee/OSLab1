#include "token.hh"

#include <string>
#include <iostream>
#include <sstream>


namespace oslab1::shell::token {

    auto get_tokens(std::string_view line) -> std::vector<std::unique_ptr<token::Token>> {
        std::vector<std::unique_ptr<token::Token>> tokens;

        std::stringstream line_stream(line.data());
        std::string split;

        bool command_process = false;
        while (std::getline(line_stream, split, ' ')) {
            if(split == "") continue;

            if (split == "exit" && !command_process) {
                tokens.emplace_back(new Token(Token::Kind::Exit));
                break;
            } else if (split == "|") {
                tokens.emplace_back(new PipeToken());
                command_process = false;
                continue;
            }
            
            tokens.emplace_back(
                new IdentToken(split, 
                    command_process ? IdentToken::Type::Arg : IdentToken::Type::Cmd));

            if (!command_process) command_process = true;
        }

        return tokens;
    }
}