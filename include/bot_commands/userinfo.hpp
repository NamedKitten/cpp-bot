void userinfo_command(json jmessage,
                      discordpp::Bot *bot) {

  std::string user_id = jmessage["author"]["id"].get<std::string>();
  std::string channel_id = jmessage["channel_id"].get<std::string>();
  if (jmessage["mentions"].size() > 0) {
    jmessage["author"] = jmessage["mentions"][0];
    user_id = jmessage["author"]["id"];
  }
  bot->call("/channels/" + channel_id, {}, "GET", [user_id, channel_id, jmessage](discordpp::Bot *bot, json msg) {
  std::string gid = msg["guild_id"].get<std::string>();
  bot->call("/guilds/" + gid + "/members/" + user_id, {}, "GET", [user_id, channel_id, jmessage, gid](discordpp::Bot *bot, json msg) {

  json mem = msg;


  Embed em;
  std::cout << "ab" << '\n';
  std::string user_id_c_str = jmessage["author"]["id"].get<std::string>();
  uint64_t user_id_int = std::strtoull(user_id_c_str.c_str(), nullptr, 10);
std::cout << "abc" << '\n';
    std::string username = jmessage["author"]["username"].get<std::string>();
  std::string avatar = jmessage["author"]["avatar"].get<std::string>();
  std::string full_name = username + "#" +
                   jmessage["author"]["discriminator"].get<std::string>();
  std::string avatar_small =
      "https://cdn.discordapp.com/avatars/" + user_id + "/" + avatar + ".png?size=256";
  std::string avatar_large =
      "https://cdn.discordapp.com/avatars/" + user_id + "/" + avatar + ".png";
  em.set_thumbnail(avatar_small);
  em.set_author(full_name, "", avatar_small);

  json pre = bot->presences_[user_id];

  std::string stata = pre["status"].get<std::string>();
  if (stata == "dnd") {
    stata = "Do Not Disturb";
  } else if (stata == "online") {
    stata = "Online";
  } else if (stata == "idle") {
    stata = "Idle";
  } else {
    stata = "Offline";
  }

  json gam = pre["game"];
  std::string game = "";
  if (gam["type"] == 0) {
    game = "Playing " + gam["name"].get<std::string>();
  } else if (gam["type"] == 1) {
    game = "Streaming " + gam["name"].get<std::string>();
  } else {
    game = "None";
  }
  em.set_description("**Display name**: " + username + "\n" + "**ID**:" +
                     " " + user_id + "\n" + emlink("Avatar", avatar_large) + "\n" +
                     bold("Status") + ": " + stata + "\n" + bold("Game") +
                     ": " + game);
  em.add_field("Join dates",
               "**This server**: " +
                   conversions::time_diff(conversions::timestamp_to_unix(mem["joined_at"].get<std::string>())) +
                   "\n**Discord**: " + conversions::time_diff(conversions::snowflake_to_unix(user_id_int)),
               true);
  if (mem["roles"].size() > 0) {
  std::stringstream roles_ss;
  for(json role : mem["roles"]){
    roles_ss << "<@&" + role.get<std::string>() + ">, ";
  }
  std::string roles = roles_ss.str();
  if (boost::algorithm::ends_with(roles, ", ")) {
    roles.erase(roles.length()-2);
  }
  em.add_field("Roles", roles, true);
  }

  bot->call("/channels/" + channel_id + "/messages", {{"embed", em.data}}, "POST");

});
});
}
