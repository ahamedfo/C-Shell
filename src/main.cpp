#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <filesystem>

std::string get_path(std::string command) {
  std::string path_env = std::getenv("PATH");
  std::stringstream ss(path_env);

  std::string path;

  while(!ss.eof()) {
    getline(ss, path, ':');
    std::string abs_path = path + '/' + command;

    if (std::filesystem::exists(abs_path)) {
      return abs_path;
    }
  }

  return "";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage  
  std::cout << "$ ";
  std::string input;
  while(std::getline(std::cin, input)){
    if (input.find("echo ") == 0 ){
      const int ECHO_LEN = 5;
      std::string text = input.substr(ECHO_LEN);
      std::cout << text << std::endl;
    } else if (input.find("exit") == 0){
      return 0;
    } else if (input.find("type") == 0) {
      std::string validTypes = input.substr(5);
      if (validTypes == "echo" || validTypes == "exit" || validTypes == "type") {
        std::cout << validTypes << " is a shell builtin" << std::endl;
      } else {
        //std::cout << validTypes << ": not found" << std::endl; 
        std::string path = get_path(validTypes);
        if (!path.empty()){
          std::cout << validTypes << " is " << path << std::endl;
        } else {
          std::cout << validTypes << ": not found" << std::endl;
        }
      }
    } else {
      std::cout << input << ": command not found" << std::endl;
    }
    std::cout << "$ ";
  }
  exit(0);
}
