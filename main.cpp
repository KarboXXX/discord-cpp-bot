#include <iostream>
#include <cstdio>
#include <array>
#include <memory>
#include <unistd.h>
#include <stdexcept>
#include <bits/stdc++.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <sys/types.h>
#include <fstream>
#include <string>
#include <cmath>

#include "./headers/presences.hpp"
#include "./headers/avatar-manipulation.cpp"
#include "./headers/extra-responses.hpp"

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

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args ) {
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 );
}

int main() {
    srand(time(0));
    std::ifstream webhook_main_updates_channel ("updates1-webhook.txt");
    std::ifstream webhook_second_updates_channel ("updates2-webhook.txt");
    std::string updates_webhook_string, updates2_webhook_string;
    dpp::webhook main_updates;
    dpp::webhook ohio_updates;
    if (webhook_main_updates_channel.is_open() && webhook_main_updates_channel.good()) {
        webhook_main_updates_channel >> updates_webhook_string;
        main_updates = dpp::webhook(updates_webhook_string);
    } else {
        std::cout << "no webhook main update url file" << std::endl;
    }
    if (webhook_second_updates_channel.is_open() && webhook_second_updates_channel.good()) {
        webhook_second_updates_channel >> updates2_webhook_string;
        ohio_updates = dpp::webhook(updates2_webhook_string);
    } else {
        std::cout << "no webhook ohio's update url file" << std::endl;
    }
    std::fstream tokenfile ("token.txt");
    std::string token;
    if (tokenfile.is_open() && tokenfile.good()) {
        tokenfile >> token;
    } else {
        std::cout << "file cointaining token either doesn't exist or don't have read permissions." << std::endl;
	std::ofstream outfile("token.txt");
	outfile << "your-token-here";
	outfile.close();
	std::cout << "a token file was created, replace all contents in the file with your token." << std::endl;
        exit(1);
    }

    dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_message_create([&bot, main_updates, ohio_updates] (const dpp::message_create_t &event) {
        if ((event.msg.content.find("bot") != std::string::npos &&
            (event.msg.content.find("shutdown") != std::string::npos ||
            event.msg.content.find("die") != std::string::npos)) &&
            (event.msg.author.id).operator nlohmann::json() == AUTHOR_ID) {
            bot.message_create(dpp::message(event.msg.channel_id, "as you wish.")
                .set_reference(event.msg.id));
            sleep(5);
            bot.shutdown();
        }

        if ((event.msg.content.find("bot") != std::string::npos && event.msg.content.find("recompile") != std::string::npos) &&
            (event.msg.author.id).operator nlohmann::json() == AUTHOR_ID) {
            bot.message_create(dpp::message(event.msg.channel_id, "as you wish.")
                .set_reference(event.msg.id));
            sleep(2);
            std::string recompile = exec("make recompile");
            sleep(10);
            if (recompile.find("Leaving") != std::string::npos || recompile.find("Error") != std::string::npos) {
                dpp::message embed = dpp::message().add_embed(dpp::embed()
                        .set_color(dpp::colors::red)
                        .set_title(bot.me.username + " faced a recompile error")
                        .add_field("Compile error!", "Something went wrong recompiling my code. Look at the terminal for more information.", true)
                        .add_field("Verified.", "Only authorized webhooks can send this message.", true)
                        .set_thumbnail(bot.me.get_avatar_url())
                    );
                bot.execute_webhook_sync(main_updates, embed);
                bot.execute_webhook_sync(ohio_updates, embed);
            }
            sleep(2);
            bot.shutdown();
        }

        Extras extra;
        extra.responses(event, bot);

        if (event.msg.content == PREFIX + "help") {
            dpp::embed embed = dpp::embed().
                set_color(dpp::colors::sti_blue).
                set_title("Hey, i got some commands!").
                set_description("Looks like you've waited a decade to this help be developed! so here i go.").
                add_field(
                        "I have",
                        "/rgb /couple /gaytest /putatest and /notify-karbox-pc go testing ima fix this message later."
                ).
                set_timestamp(time(0)).
                set_footer(dpp::embed_footer().set_icon(event.msg.author.get_avatar_url()).set_text(event.msg.author.username));
            bot.message_create(dpp::message(event.msg.channel_id, embed).set_reference(event.msg.id));
        }
    });

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {

      if (event.command.get_command_name() == "sequence") {
	double terms[] = {
	  std::get<double>(event.get_parameter("termo-1")),
	  std::get<double>(event.get_parameter("termo-2")),
	  std::get<double>(event.get_parameter("termo-3"))
	};
	int n = std::get<int64_t>(event.get_parameter("n-termo"));
	float r;
	float r1 = (float)terms[1] - (float)terms[0];
	float r2 = (float)terms[2] - (float)terms[1];
	int rr1, rr2;
	bool isPA, validSequence, rIsReal;
    
	if (r1 == r2) {
	  r = r1;
	  isPA = true;
	  validSequence = true;
	  if (r != std::round(r)) { rIsReal = true; } else { rIsReal = false; }
	} else {
	  r1 = (float)terms[1] / (float)terms[0];
	  r2 = (float)terms[2] / (float)terms[1];
	  
	  rr1 = static_cast<int>(terms[1]) % static_cast<int>(terms[0]);
	  rr2 = static_cast<int>(terms[2]) % static_cast<int>(terms[1]);
	    
	  if (r1 == r2) {
	    r = r1;
	    isPA = false;
	    validSequence = true;
	    if (rr1 == 0 && rr2 == 0) { rIsReal = false; } else { rIsReal = true; }
	  } else {
	    validSequence = false;
	  }
	  
	}
	
	float an;
	if (isPA && validSequence) {
	  an = (float)terms[0] + (n - 1) * r;
	  event.reply(string_format("O %i termo é igual a %.4f de razão r=%.2f\n", n, an, r));
	} else if (!isPA && validSequence) {
	  an = (float)terms[0] * std::pow(r, n-1);
	  event.reply(string_format("O %i termo é igual a %.4f de razão q=%.4f, (%.0f de resto %i)\n", n, an, r, std::round(r), rr1));
	} else if (!validSequence) {
	  event.reply("A sequência não existe, razão entre termos é irregular.");
	}
      }

      if (event.command.get_command_name() == "putatest") {
	auto who = std::get<dpp::snowflake>(event.get_parameter("who"));
	auto whoUser = event.command.get_resolved_user(who);
	dpp::user msgauthor = event.command.get_issuing_user();

	std::string usermention = whoUser.get_mention();
	dpp::snowflake id = whoUser.id;
	std::string avatar = whoUser.get_avatar_url();

	if (id.operator nlohmann::json() == "851214582122283048") {
	  dpp::embed embed = dpp::embed().set_color(dpp::colors::black)
	    .set_description(usermention + std::string(" é 100% **puta**."))
	    .set_timestamp(time(0))
	    .set_title("100% PIRANHA")
	    .set_thumbnail(avatar)
	    .set_footer(dpp::embed_footer().set_icon(msgauthor.get_avatar_url()).set_text(msgauthor.username));
	  event.reply(dpp::message().add_embed(embed));
	} else {
	  int puta = std::rand() % 100;
	  dpp::embed embed = dpp::embed().set_color(dpp::colors::red)
	    .set_description(usermention + std::string(" é " + std::to_string(puta) + "% puta/puto."))
	    .set_timestamp(time(0))
	    .set_title(std::to_string(puta) + "% Piranho")
	    .set_thumbnail(avatar)
	    .set_footer(dpp::embed_footer().set_icon(msgauthor.get_avatar_url()).set_text(msgauthor.username));
	  event.reply(dpp::message().add_embed(embed));
	}

      }
      if (event.command.get_command_name() == "gaytest") {
	auto who = std::get<dpp::snowflake>(event.get_parameter("who"));
	auto whoUser = event.command.get_resolved_user(who);
	dpp::user msgauthor = event.command.get_issuing_user();

	std::string usermention = whoUser.get_mention();
	dpp::snowflake id = whoUser.id;
	std::string avatar = whoUser.get_avatar_url();

	if (id.operator nlohmann::json() == AUTHOR_ID) {
	  dpp::embed embed = dpp::embed().set_color(dpp::colors::black)
	    .set_description(usermention + std::string(" é 0% gay."))
	    .set_timestamp(time(0))
	    .set_title("100% HETERO")
	    .set_thumbnail(avatar)
	    .set_footer(dpp::embed_footer().set_icon(msgauthor.get_avatar_url()).set_text(msgauthor.username));
	  event.reply(dpp::message().add_embed(embed));
	} else if (id.operator nlohmann::json() == "381637890679111692") { // reynan
	  dpp::embed embed = dpp::embed().set_color(dpp::colors::pink)
	    .set_description(usermention + std::string(" é 0% hétero."))
	    .set_timestamp(time(0))
	    .set_title("100% GAY!!!")
	    .set_thumbnail(avatar)
	    .set_footer(dpp::embed_footer().set_icon(msgauthor.get_avatar_url()).set_text(msgauthor.username));
	  event.reply(dpp::message().add_embed(embed));
	} {
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

      if (event.command.get_command_name() == "rgb") {
	dpp::user author = event.command.get_issuing_user();
	int64_t r = std::get<int64_t>(event.get_parameter("red"));
	int64_t g = std::get<int64_t>(event.get_parameter("green"));
	int64_t b = std::get<int64_t>(event.get_parameter("blue"));
	int64_t a = std::get<int64_t>(event.get_parameter("alpha"));

	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;
	if (a > 1) a = 1;

	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;
	if (a < 0) a = 0;
	  
	Avatar avatar;
	auto rgba = avatar.createRGBA(r, g, b, a);
	auto rgb = avatar.createRGB(r, g, b);
	  	  
	event.reply(dpp::message()
		    .add_embed(dpp::embed()
			       .set_color(rgb)
			       .set_title("Prontinho.")
			       .set_timestamp(time(0))
			       .add_field("RGB", std::to_string(rgb), true)
			       .add_field("RGBA", std::to_string(rgba), true)
			       )
		    );
      }
	
      if (event.command.get_command_name() == "couple") {
	dpp::user user1 = event.command.get_resolved_user(std::get<dpp::snowflake>(event.get_parameter("user1")));
	dpp::user user2 = event.command.get_resolved_user(std::get<dpp::snowflake>(event.get_parameter("user2")));
	std::string half_user1 = user1.username.substr(0, round(user1.username.length()/2));
	std::string half_user2 = user2.username.substr(round(user2.username.length()/2));
	    
	int porcent = round(std::rand() % 100);
	std::string loading_ascii = "▰";
	std::string background_ascii = "▱";
	std::string progress_ascii = "";
	std::uint32_t color = dpp::colors::red;
	char* emoji = (char*)"./media/vomiting.png";
	    
	for (int i=0; i<10;i++) {
	  if (i<(porcent/10)) progress_ascii += loading_ascii;
	  if (i>(porcent/10)) progress_ascii += background_ascii;
	}
	    
	if (porcent >= 35) emoji = (char*)"./media/thumbsup.png";
	if (porcent >= 50) color = dpp::colors::forest_green;
	if (porcent >= 60) { color = dpp::colors::lime; emoji = (char*)"heart"; }

	Avatar avatar;
	event.thinking();
	bool hasDefaultAvatar = (user1.get_avatar_url().find("embed") != std::string::npos) ||
	  (user2.get_avatar_url().find("embed") != std::string::npos);
	std::string result = avatar.run(user1.get_avatar_url(), user2.get_avatar_url(), emoji, hasDefaultAvatar);
	dpp::message answer = dpp::message().add_embed(dpp::embed().set_color(color)
						       .set_title("Quão compatível será esse casal?")
						       .add_field("Casal", user1.get_mention() + " e " + user2.get_mention(), true)
						       .add_field("Compatibilidade", std::string(half_user1+half_user2)+" : **"+progress_ascii+"** "+std::to_string(porcent)+"%", true)
						       .set_image("attachment://" + result)
						       .set_timestamp(time(0))
						       .set_footer(dpp::embed_footer()
								   .set_icon(event.command.get_issuing_user().get_avatar_url())
								   .set_text(event.command.get_issuing_user().username)
								   )
						       ).add_file(result, dpp::utility::read_file(result));
	event.edit_original_response(answer);
	remove(result.c_str());
      }
    });

    bot.on_ready([&bot, main_updates, ohio_updates](const dpp::ready_t& event) {
      dpp::message embed = dpp::message().add_embed(dpp::embed()
						    .set_color(dpp::colors::green)
						    .set_title(bot.me.username + " is online now.")
						    .add_field("Bot is ready!", "if you see this message more than once in 3 minutes, KarboXXX is probably having a hard time debugging me.", true)
						    .add_field("Verified.", "Only authorized webhooks can send this message.", true)
						    .set_thumbnail(bot.me.get_avatar_url())
						    );
      bot.execute_webhook_sync(main_updates, embed);
      bot.execute_webhook_sync(ohio_updates, embed);

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
	dpp::slashcommand rgb("rgb", "gera um número inteiro, que vale um código RGB(A).", bot.me.id);
	rgb.add_option( dpp::command_option(dpp::co_integer, "red", "Valor vermelho entre 0 e 255", true) );
	rgb.add_option( dpp::command_option(dpp::co_integer, "green", "Valor verde entre 0 e 255", true) );
	rgb.add_option( dpp::command_option(dpp::co_integer, "blue", "Valor azul entre 0 e 255", true) );
	rgb.add_option( dpp::command_option(dpp::co_integer, "alpha", "Valor de opacidade entre 0 e 1", true) );
	
	dpp::slashcommand gaytest("gaytest", "mostra o quão gay alguém é.", bot.me.id);
        gaytest.add_option( dpp::command_option(dpp::co_user, "who", "Quem?", true) );

        dpp::slashcommand putatest("putatest", "mostra o quão puta alguém é.", bot.me.id);
        putatest.add_option( dpp::command_option(dpp::co_user, "who", "Quem?", true) );

	dpp::slashcommand papg("sequence", "calcula PA ou PG de uma sequência.", bot.me.id);
	papg.add_option( dpp::command_option(dpp::co_number, "termo-1", "Primeiro termo", true) );
	papg.add_option( dpp::command_option(dpp::co_number, "termo-2", "Segundo termo", true) );
	papg.add_option( dpp::command_option(dpp::co_number, "termo-3", "Terceiro termo", true) );
	papg.add_option( dpp::command_option(dpp::co_integer, "n-termo", "Termo que você quer descobrir", true) );
	
        dpp::slashcommand notify("notify-karbox-pc", "manda uma notificação pro computador do karbox.", bot.me.id);
        notify.add_option( dpp::command_option(dpp::co_string, "notification", "Notificação em texto.", true) );

        dpp::slashcommand couple("couple", "Quão bem esse casal daria?", bot.me.id);
        couple.add_option( dpp::command_option(dpp::co_user, "user1", "Primeira pessoa", true) );
        couple.add_option( dpp::command_option(dpp::co_user, "user2", "Segunda pessoa", true) );

        bot.global_command_create(couple);
        bot.global_command_create(gaytest);
        bot.global_command_create(putatest);
        bot.global_command_create(notify);
	bot.global_command_create(rgb);
        bot.global_command_create(papg);
// bot.global_bulk_command_create({couple, gaytest, putatest, notify, rgb});
      }
    });

    bot.start(dpp::st_wait);
    return 0;
}
