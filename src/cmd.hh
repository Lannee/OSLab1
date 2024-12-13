#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <optional>
#include <ranges>
#include <string_view>

#include "token.hh"
#include "pipe.hh"

namespace oslab1::shell::cmd {

class Cmd {
public:
    using arg = std::string;

    Cmd(std::string_view file, bool exit_cmd = false) 
        : executable_file_(file), exit_cmd_(exit_cmd), input_pipe_(std::nullopt), output_pipe_(std::nullopt) {}

    Cmd(std::string_view file, 
        std::shared_ptr<Pipe> input_pipe , 
        std::shared_ptr<Pipe> output_pipe,
        bool exit_cmd = false) 
        : executable_file_(file), exit_cmd_(exit_cmd), input_pipe_(input_pipe), output_pipe_(output_pipe) {}

public:
    auto run() -> void;

    auto append_arg(std::string_view arg) { args_.emplace_back(std::string(arg).c_str()); }

    auto set_input_pipe (std::shared_ptr<Pipe> input_pipe)  { input_pipe_  = std::optional(input_pipe);  }
    auto set_output_pipe(std::shared_ptr<Pipe> output_pipe) { output_pipe_ = std::optional(output_pipe); }

    auto get_exec_file() const { return executable_file_; }
    auto get_args() const -> std::vector<arg> const & { return args_; }

private:

    auto get_args_char() const -> std::vector<const char *>;
    auto get_execv_args_char() const -> std::vector<const char *>;

    std::string executable_file_;
    std::vector<arg> args_;

    const bool exit_cmd_;

    std::optional<std::shared_ptr<Pipe>> input_pipe_;
    std::optional<std::shared_ptr<Pipe>> output_pipe_;
};

auto from_tokens(std::vector<std::unique_ptr<token::Token>> const &tokens) 
    -> std::vector<Cmd>;

};