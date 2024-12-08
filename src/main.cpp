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

std::vector<std::string> split_string(const std::string &inputString, char delimeter) {
  std::stringstream ss(inputString);
  std::vector<std::string> return_vect;
  std::string token;
  while(getline(ss,token, delimeter)){
    return_vect.push_back(token);
  }
  return return_vect;
}

void get_pwd(){
  std::filesystem::path cwd = std::filesystem::current_path();
  std::cout << cwd;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string path_string = getenv("PATH");
  std::stringstream ss1(path_string);
  std::vector<std::string> program_paths = split_string(path_string, ':');

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
        std::string path = get_path(validTypes);
        if (!path.empty()){
          std::cout << validTypes << " is " << path << std::endl;
        } else {
          std::cout << validTypes << ": not found" << std::endl;
        }
      }
    } else if ( input.find("pwd") == 0) {
      get_pwd()
    } else {
      int command_idx = input.find(' ');
      std::string command = input.substr(0, command_idx);
      std::string filepath;
      for(int i = 0; i < program_paths.size(); i++){
        std::string cur_program = program_paths[i];
        std::string abs_path = cur_program + '/' + command;
        if(std::filesystem::exists(abs_path)){
          std::string exec_path = "exec " + cur_program + '/' + input;
          std::system(exec_path.c_str());
          break;
        } else if ( i == program_paths.size() - 1) {
          std::cout << command << ": command not found" << std::endl;
        }
      }
    }
    std::cout << "$ ";
  }
  exit(0);
}
