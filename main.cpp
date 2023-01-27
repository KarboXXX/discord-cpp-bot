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
const std::string PREFIX = "++";

std::string exec(std::string command) {
   char buffer[128];
   std::string result = "";
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) return "popen failed!";
   while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != NULL) result += buffer;
   }
   pclose(pipe);
   return result;
}

int main() {
    std::ifstream tokenfile ("token.txt");
    std::string token;
    if (tokenfile.is_open() && tokenfile.good()) {
        tokenfile >> token;
    } else {
        std::cout << "file cointaining token either doesn't exist or don't have read permissions. create or fix token.txt file.";
        exit(1);
    }

    dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_message_create([&bot] (const dpp::message_create_t & event) {
        if (event.msg.content == "javascript") bot.message_create(dpp::message(event.msg.channel_id, "high-level").set_reference(event.msg.id));

        if (event.msg.content == "python") bot.message_create(dpp::message(
            event.msg.channel_id, "high-level as shit")
            .set_reference(event.msg.id));

        if (event.msg.content == "C#") bot.message_create(dpp::message(
            event.msg.channel_id, "high-level as shit, but Windows")
            .set_reference(event.msg.id));

        if (event.msg.content == "Java") bot.message_create(dpp::message(
            event.msg.channel_id, "high-level as s... oh minecraft!")
            .set_reference(event.msg.id));

        if (event.msg.content == "Linux") bot.message_create(dpp::message(
            event.msg.channel_id, "I KNOW RIGHT?? :smiley:")
            .set_reference(event.msg.id));

        if (event.msg.content == "rafaela") bot.message_create(dpp::message(
            event.msg.channel_id, "ah n essa puta dnv?")
            .set_reference(event.msg.id));

        if (event.msg.content == PREFIX + "help") {
            dpp::embed embed = dpp::embed().
                set_color(dpp::colors::sti_blue).
                set_title("Hey, i'm sorry there...").
                set_description("I ain't have a help message still, wait for a couple more decades i might give you an answer.").
                add_field(
                        "I'm not gonna lie...",
                        "It is pretty boring coding those MAKEFILES to compile my code, no cap. Would prefer Python, if it wasn't that high level. ((javascript"
                ).
                set_timestamp(time(0)).
                set_footer(dpp::embed_footer().set_icon(event.msg.author.get_avatar_url()).set_text(event.msg.author.username));
            bot.message_create(dpp::message(event.msg.channel_id, embed).set_reference(event.msg.id));
        }
    });

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "gaytest") {
            auto who = std::get<dpp::snowflake>(event.get_parameter("who"));
            dpp::user* whoUser = dpp::find_user(who);
            dpp::user msgauthor = event.command.get_issuing_user();

            std::string usermention = whoUser->get_mention();
            dpp::snowflake id = whoUser->id;
            std::string avatar = whoUser->get_avatar_url();

            if (id.operator nlohmann::json() == AUTHOR_ID) {
                dpp::embed embed = dpp::embed().set_color(dpp::colors::black)
                        .set_description(usermention + std::string(" é 0% gay."))
                        .set_timestamp(time(0))
                        .set_title("100% HETERO")
                        .set_thumbnail(avatar)
                        .set_footer(dpp::embed_footer().set_icon(msgauthor.get_avatar_url()).set_text(msgauthor.username));
                event.reply(dpp::message().add_embed(embed));
            } else {
                int gay = std::rand() % 100;
                //event.reply(usermention + " é " + gay + "% gay.");
                dpp::embed embed = dpp::embed().set_color(dpp::colors::endeavour)
                        .set_description(usermention + std::string(" é " + std::to_string(gay) + "% gay."))
                        .set_timestamp(time(0))
                        .set_title(std::to_string(100 - gay) + "% Hétero")
                        .set_thumbnail(avatar)
                        .set_footer(dpp::embed_footer().set_icon(msgauthor.get_avatar_url()).set_text(msgauthor.username));
                event.reply(dpp::message().add_embed(embed));
            }
        }
        if (event.command.get_command_name() == "notify-karbox-pc") {
            std::string message = std::get<std::string>(event.get_parameter("notification"));
            dpp::user whosent = event.command.get_issuing_user();
            dpp::embed embed = dpp::embed().set_color(dpp::colors::green)
                .set_title("Message sent!")
                .set_timestamp(time(0))
                .add_field("Note", "All especial characteres that could break the syntax function of the notification call, will be deleted.");
            event.reply(dpp::message().add_embed(embed));

            std::replace(message.begin(), message.end(), '\'', char(' '));
            std::replace(message.begin(), message.end(), '"', char(' '));
            std::replace(message.begin(), message.end(), ';', char(' '));
            std::replace(message.begin(), message.end(), '>', char(' '));

            exec("notify-send 'C++ Discord Project' '" + whosent.username + " sent: " + message + "'");
        }
        if (event.command.get_command_name() == "couple") {
            dpp::user user1 = event.command.get_resolved_user(std::get<dpp::snowflake>(event.get_parameter("user1")));
            dpp::user user2 = event.command.get_resolved_user(std::get<dpp::snowflake>(event.get_parameter("user2")));
            std::string half_user1 = user1.username.substr(0, user1.username.length()/2);
            std::string half_user2 = user2.username.substr(user2.username.length()/2);

            int porcent = std::rand() % 100;
            std::string loading_ascii = "█▒▒▒▒▒▒▒▒▒";
            std::uint32_t color = dpp::colors::red;

            if (porcent >= 15) {
                loading_ascii = "███▒▒▒▒▒▒▒";
            }
            if (porcent >= 50) {
                loading_ascii = "█████▒▒▒▒▒";
                color = dpp::colors::forest_green;
            }
            if (porcent >= 70) {
                loading_ascii = "███████▒▒▒";
                color = dpp::colors::lime;
            }
            if (porcent >= 100) {
                loading_ascii = "██████████";
            }
            event.reply(dpp::message().add_embed(dpp::embed().set_color(color)
                .set_title("Quão compatível será esse casal?")
                .set_description(std::string(half_user1+half_user2)+" : **"+loading_ascii+"** "+std::to_string(porcent)+"%")
                .set_timestamp(time(0))
                .set_footer(dpp::embed_footer()
                    .set_icon(event.command.get_issuing_user().get_avatar_url())
                    .set_text(event.command.get_issuing_user().username)
                )
            ));
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        class Presences {
            public:
                void servers_type(dpp::cluster &r_bot) {
                    r_bot.current_user_get_guilds([&](const dpp::confirmation_callback_t &callback) {
                        if (!callback.is_error()) {
                            dpp::guild_map guildMap = std::get<dpp::guild_map>(callback.value);
                            int gs = 0;
                            for (auto g = guildMap.begin(); g != guildMap.end(); ++g) {gs++;}
                            r_bot.set_presence(dpp::presence(dpp::presence_status::ps_online, dpp::activity_type::at_listening,
                                    std::to_string(gs) + std::string(" servers.")
                                )
                            );
                        }
                    });
                }

                void default_type(dpp::cluster &r_bot) {
                    r_bot.set_presence(dpp::presence(dpp::presence_status::ps_dnd, dpp::activity_type::at_watching, "KarboXXX code nonstop."));
                }

                void suggestions_type(dpp::cluster &r_bot) {
                    r_bot.set_presence(dpp::presence(dpp::presence_status::ps_idle, dpp::activity_type::at_listening, "suggestions..."));
                }
        };

        Presences presences;
        presences.default_type(bot);

        bot.start_timer([&](const dpp::timer &timing) {
            Presences presences;
            int random_selected = (std::rand() % 3);
            switch (random_selected) {
                default: presences.default_type(bot);
                case 0: presences.servers_type(bot);
                    break;
                case 1: presences.default_type(bot);
                    break;
                case 2: presences.suggestions_type(bot);
                    break;
            }
        }, 10);

      if (dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand gaytest("gaytest", "mostra o quão gay você é.", bot.me.id);
        gaytest.add_option( dpp::command_option(dpp::co_user, "who", "Quem?", true) );

        dpp::slashcommand notify("notify-karbox-pc", "manda uma notificação pro computador do karbox.", bot.me.id);
        notify.add_option( dpp::command_option(dpp::co_string, "notification", "Notificação em texto.", true) );

        dpp::slashcommand couple("couple", "Quão bem esse casal daria?", bot.me.id);
        couple.add_option( dpp::command_option(dpp::co_user, "user1", "Primeira pessoa", true) );
        couple.add_option( dpp::command_option(dpp::co_user, "user2", "Segunda pessoa", true) );

        bot.global_command_create(couple);
        bot.global_command_create(gaytest);
        bot.global_command_create(notify);
      }
    });

    bot.start(dpp::st_wait);
}
