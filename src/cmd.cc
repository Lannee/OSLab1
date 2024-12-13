#include <unistd.h>
#include <cassert>
#include <sys/wait.h>

#include "cmd.hh"
#include "errors.hh"

namespace oslab1::shell::cmd {

auto from_tokens(std::vector<std::unique_ptr<token::Token>> const &tokens) 
    -> std::vector<Cmd> {

    std::vector<Cmd> commands;
    
    Cmd *cmd = nullptr;
    std::optional<std::shared_ptr<Pipe>> cmd_input_pipe = std::nullopt;

    bool end = false; // commands tarnslation dead end
    for (const auto &token : tokens) {
        switch(token->getKind()) {
            using enum token::Token::Kind;
        case Exit: {
            end = true;
            commands.emplace_back("exit", true);
            break;
        }
        case Ident: {
            const auto ident_token = static_cast<token::IdentToken*>(token.get());
            switch (ident_token->getType()) {
                using enum token::IdentToken::Type;
            case Cmd: {
                commands.emplace_back(ident_token->getIdent());
                cmd = &commands.back();
                if (cmd_input_pipe)
                    cmd->set_input_pipe(*cmd_input_pipe);
                
                cmd_input_pipe = std::nullopt; // set default for next cmd
                break;
            }
            case Arg: {
                assert(cmd);
                cmd->append_arg(ident_token->getIdent());
                break;
            }
            }
            break;
        }
        case Pipe: {
            const auto pipe_token = static_cast<token::PipeToken*>(token.get());

            if(!cmd) throw ParsingError("Invalid pipe position");

            cmd_input_pipe = pipe_token->get_pipe();
            cmd->set_output_pipe(pipe_token->get_pipe());
        }

        }
        
        if (end) break; // stop translating commands that won't be executed
    }

    return commands;
}

auto Cmd::get_args_char() const -> std::vector<const char *> {
    std::vector<const char *> args_char(args_.size());

    for (size_t i = 0; i < args_.size(); i++) {
        args_char[i] = args_[i].c_str();
    }

    return args_char;
}

auto Cmd::get_execv_args_char() const -> std::vector<const char *> {
    auto args = get_args_char();
    args.insert(args.begin(), executable_file_.c_str());
    args.emplace_back(nullptr);
    return args;
}

auto Cmd::run() -> void {
    if (exit_cmd_) exit(0);

    pid_t pid = fork();

    if (pid == 0) {
        if (input_pipe_) {
            dup2((*input_pipe_)->get_read_end(), STDIN_FILENO);
            (*input_pipe_)->close_read_end();
            (*input_pipe_)->close_write_end();
        }

        if (output_pipe_) {
            dup2((*output_pipe_)->get_write_end(), STDOUT_FILENO);
            (*output_pipe_)->close_write_end();
            (*output_pipe_)->close_read_end();
        }

        const auto args = get_execv_args_char();

        execvp(executable_file_.c_str(), (char* const*)args.data());
        perror("Ошибка execvp.");
        exit(1);
    } else {
        if (input_pipe_)  (*input_pipe_)->close_read_end();
        if (output_pipe_) (*output_pipe_)->close_write_end();

        waitpid(pid, nullptr, 0); 
    }
}


} // namespace oslab1::shell:cmd
