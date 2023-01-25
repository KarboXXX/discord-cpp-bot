#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

// visit dpp.dev for this dependency ;)
#include <dpp/dpp.h>

const std::string AUTHOR_ID = "708857740965183559";

int main() {
    std::ifstream tokenfile ("token.txt");
    std::string token;
    if (tokenfile.fail()) {
        std::cout << "file cointaining token either doesn't exist or don't have read permissions. create or fix token.txt file.";
        exit(1);
    }
    if (tokenfile.is_open() && tokenfile.good()) {
	tokenfile >> token;
    }

    dpp::cluster bot(token);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "rafaela") {
            event.reply("submissa");
        }
        if (event.command.get_command_name() == "gaytest") {
            auto who = std::get<dpp::snowflake>(event.get_parameter("who"));
            std::string usermention = (event.command.get_resolved_user(who)).get_mention();
            dpp::snowflake id = (event.command.get_resolved_member(who)).user_id;
            if (id.operator nlohmann::json() == AUTHOR_ID) {
                event.reply(usermention + std::string(" é 0% gay."));
            } else {
                std::string gay = std::to_string(std::rand() % 102);
                event.reply(usermention + " é " + gay + "% gay.");
            }
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
	        dpp::slashcommand rafaela("rafaela", "mostra o que a rafaela realmente é.", bot.me.id);
            dpp::slashcommand gaytest("gaytest", "mostra o quão gay você é.", bot.me.id);
            gaytest.add_option(
                dpp::command_option(dpp::co_user, "who", "Quem?", true)
            );
	        bot.global_command_create(rafaela);
            bot.global_command_create(gaytest);
        }
    });
    
    bot.start(dpp::st_wait);
}
