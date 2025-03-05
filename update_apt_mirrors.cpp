#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <unistd.h>

using std::cout, std::endl;
using std::string, std::string_view;
using std::filesystem::exists;
using std::filesystem::path;

const path mirrors_file_path{"/etc/apt/sources.list"};
const std::regex url_regex{"https?\\:\\/\\/\\S*"};
string_view default_mirror{"http://archive.debian.petiak.ir/debian"};

void print_usage() {
  cout << "set_apt_mirror <mirror>\n";
  cout << "YOU MUST RUN ME AS ROOT\n";
}

int main(int argc, const char **argv) {
  if (getuid() != 0) {
    print_usage();
    return -1;
  }
  if (argc == 2) {
    default_mirror = argv[1];
  }
  if (!exists(mirrors_file_path)) {
    cout << "No mirror file exists!\nWeired :-/\n";
    return -2;
  }
  std::fstream mirror_file_stream{mirrors_file_path};
  if (!mirror_file_stream.is_open()) {
    cout << "Can't open mirror files.\n";
    return -3;
  }
  string line{""};
  std::ofstream temp_file{"temp_file"};
  if (!temp_file) {
    cout << "Can't open a temp file\n";
    return -4;
  }
  while (!mirror_file_stream.eof()) {
    std::getline(mirror_file_stream, line);
    if (line.find("#") == std::string::npos) {
      if (std::regex_search(line, url_regex)) {
        if (!std::regex_search(line, std::regex{"security"})) {
          line =
              std::regex_replace(line, url_regex, std::string{default_mirror});
        }
      }
    }
    temp_file << line << endl;
  }
  std::filesystem::rename(mirrors_file_path,
                          mirrors_file_path.string() + "-old");
  std::filesystem::rename("./temp_file", mirrors_file_path);

  mirror_file_stream.close();
  temp_file.close();

  return 0;
}
