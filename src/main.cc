#include <iostream>
#include <ranges>
#include <memory>
#include <vector>
#include <chrono>

#include "token.hh"
#include "cmd.hh"

namespace oslab1::shell {

namespace {

auto get_input(bool verbose = true) -> std::string_view {
    std::flush(std::cout);
    constexpr static auto INITIATE_PHRASE = "mysh> ";

    if (verbose) std::cout << INITIATE_PHRASE;

    std::string line; 
    std::getline(std::cin, line);

    return line;
}

} // oslab1::shell

auto run() -> int {
    while (true) {
        const auto input = get_input();

        const auto tokens = token::get_tokens(input);

        const auto start_time = std::chrono::system_clock::now();

        for (auto &command : cmd::from_tokens(tokens)) command.run();
         
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start_time;
 
        std::cout << "elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
    }

    return 0;
}

}


auto main() -> int {
    return oslab1::shell::run();
}