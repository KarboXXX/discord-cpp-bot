#include <iostream>
#include <cstdio>
#include <array>
#include <memory>
#include <stdexcept>
#include <bits/stdc++.h>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <string>

// visit dpp.dev for this dependency ;)
#include <dpp/dpp.h>


const std::string AUTHOR_ID = "708857740965183559";

std::string exec(std::string command) {
   char buffer[128];
   std::string result = "";
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }
   while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }
   pclose(pipe);
   return result;
}

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
        if (event.command.get_command_name() == "notify-karbox-pc") {
            std::string message = std::get<std::string>(event.get_parameter("notification"));
            std::string whosent = event.command.get_issuing_user().username;
            event.reply("message sent.");

            std::replace(message.begin(), message.end(), '\'', char(' '));
            std::replace(message.begin(), message.end(), '"', char(' '));
            std::replace(message.begin(), message.end(), ';', char(' '));
            std::replace(message.begin(), message.end(), '>', char(' '));

            exec("notify-send 'C++ Discord Project' '" + whosent + " sent: " + message + "'");
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
      bot.set_presence(
        dpp::presence(dpp::presence_status::ps_online,
          dpp::activity_type::at_watching,
          "KarboXXX code nonstop."
        )
      );
      if (dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand rafaela("rafaela", "mostra o que a rafaela realmente é.", bot.me.id);
        dpp::slashcommand gaytest("gaytest", "mostra o quão gay você é.", bot.me.id);
        gaytest.add_option(
            dpp::command_option(dpp::co_user, "who", "Quem?", true)
        );
        dpp::slashcommand notify("notify-karbox-pc", "manda uma notificação pro computador do karbox.", bot.me.id);
        notify.add_option(
            dpp::command_option(dpp::co_string, "notification", "Notificação em texto.", true)
        );


	      bot.global_command_create(rafaela);
        bot.global_command_create(gaytest);
        bot.global_command_create(notify);
      }
    });

    bot.start(dpp::st_wait);
}
