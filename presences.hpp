#pragma once
#include "<dpp/dpp.h>"

class Presences {
    public:
        // Rich Presence that shows how many servers the bot is currently in.
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
        // Default bot Rich Presence.
        void default_type(dpp::cluster &r_bot) {
            r_bot.set_presence(dpp::presence(dpp::presence_status::ps_dnd, dpp::activity_type::at_watching, "KarboXXX code nonstop."));
        }
        void suggestions_type(dpp::cluster &r_bot) {
        // Rich Presence showing that the bot is "listening to suggestions".
            r_bot.set_presence(dpp::presence(dpp::presence_status::ps_idle, dpp::activity_type::at_listening, "suggestions..."));
        }
};