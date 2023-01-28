#pragma once
#include "<dpp/dpp.h>"

class Extras {
    public:

        void responses(dpp::message_create_t r_event, dpp::cluster &r_bot) {
            if (r_event.msg.content == "javascript") r_bot.message_create(dpp::message(
                r_event.msg.channel_id, "high-level")
                .set_reference(r_event.msg.id));

            if (r_event.msg.content == "python") r_bot.message_create(dpp::message(
                r_event.msg.channel_id, "high-level as shit")
                .set_reference(r_event.msg.id));

            if (r_event.msg.content == "C#") r_bot.message_create(dpp::message(
                r_event.msg.channel_id, "high-level as shit, but Windows")
                .set_reference(r_event.msg.id));

            if (r_event.msg.content == "Java") r_bot.message_create(dpp::message(
                r_event.msg.channel_id, "high-level as s... oh minecraft!")
                .set_reference(r_event.msg.id));

            if (r_event.msg.content == "Linux") {
                std::string answer[] = {"I KNOW RIGHT?? :smiley:", "YOU DAMN RIGHT THO",
                    "no cap", "agreed", "chad"};
                int selected = (std::rand() % 5);
                r_bot.message_create(dpp::message(
                    r_event.msg.channel_id, answer[selected])
                    .set_reference(r_event.msg.id));
            }

            // https://tenor.com/view/prob-proboscis-monkey-nosaty-kahau-nosaty-kahau-gif-23018177
            if (r_event.msg.content == "monke villager") r_bot.message_create(dpp::message(
                r_event.msg.channel_id, 
                    "https://tenor.com/view/prob-proboscis-monkey-nosaty-kahau-nosaty-kahau-gif-23018177")
                .set_reference(r_event.msg.id));

            if (r_event.msg.content == "rafaela") {
                std::string answer[] = {"ah n essa puta dnv?", "daqui a uns anos ela responde",
                    "ouvi um puta", "desconto no programa?"};
                int selected = (std::rand() % 5);
                r_bot.message_create(dpp::message(
                    r_event.msg.channel_id, answer[selected])
                    .set_reference(r_event.msg.id));
            }
        }
};