#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <filesystem>
#include <unistd.h>
#include <format>


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
  std::string cwd = std::filesystem::current_path();
  std::cout << cwd << "\n";
}


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string path_string = getenv("PATH");
  std::string home_string = getenv("HOME");
  std::stringstream ss1(path_string);
  std::vector<std::string> program_paths = split_string(path_string, ':');

  // Uncomment this block to pass the first stage  
  std::cout << "$ ";
  std::string input;
  while(std::getline(std::cin, input)){
    int command_idx = input.find(' ');
    std::string command = input.substr(0, command_idx);
    if (input.find("echo ") == 0 ){
      const int ECHO_LEN = 5;
      std::string text = input.substr(ECHO_LEN);

      std::string result;
      bool in_quotes{false};
      bool in_single_quotes{false};
      bool was_quotes{false};
      char current_quote = '\0';
      std::string temp;

      for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];

        if ((c == '\"' || c == '\'') && !in_quotes) {
          //going into a quote
          was_quotes = true;
          if (c == '\''){
            in_single_quotes = true;
          }
          in_quotes = true;
          current_quote = c;
          //next itteration
        } else if (c == current_quote && in_quotes ) {
          //leaving the quote
          in_quotes = false;
          if (c == '\'' && in_single_quotes){
            in_single_quotes = false;
          }
          current_quote = '\0';
          //if (!result.empty()) result += ' ';
          result += temp;
          temp.clear();
        } else if (in_quotes) {
          if (c == '\\' && i + 1 < text.size() && !in_single_quotes ) {
            //std::cout << i << ' ' << i + 1  << ' ' << text.size() ;
            char next = text[i + 1];

            if (next == '\\' || next == '"' || next == '$' || next == '\n') {
                temp += next;
                if(next == '"'){
                  in_quotes = false;
                  result += temp;
                  temp.clear();
                }
                
                ++i;
            } else {
              temp += c;
            }
          } else {
            temp += c;
          }
        } else if (!in_quotes && c == ' '){
          continue;
        } else {
          if (!result.empty()) {
            result += c;
          }
        }
      }

      if(!temp.empty()) {
        if (!result.empty()) { 
          result += ' ';
          }
        result += temp;
      }

      if (was_quotes == false){
        char c;
        std::string output;
        bool inStr{false};
        for(size_t i = 0; i < text.size(); i++){
          if ( text[i] == '\\' && ( i+1 ) < text.size()){
            inStr = true;
            output += text[++i];

          } else if ( text[i] != ' ' ) {
            inStr = true;
            output += text[i];
          } else if ( text[i] == ' ' && inStr) {
            output += ' ';
            inStr = false;
          }
        }        
        std::cout << output << std::endl;    
      } else {
        std::cout << result << std::endl;
      }
    } else if (input.find("exit") == 0){
      return 0;
    } else if (command == "type") {
      std::string validTypes = input.substr(command_idx + 1);
      if (validTypes == "echo" || validTypes == "exit" || validTypes == "type" || validTypes == "pwd") {
        std::cout << validTypes << " is a shell builtin" << std::endl;
      } else {
        std::string path = get_path(validTypes);
        if (!path.empty()){
          std::cout << validTypes << " is " << path << std::endl;
        } else {
          std::cout << validTypes << ": not found" << std::endl;
        }
      }
    } else if ( command == "pwd") {
      get_pwd();
    } else if ( command == "cd") {
      std::string requested_path = input.substr(command_idx + 1);
      if (requested_path[0] == '/') {
        if ( std::filesystem::exists(requested_path) ) {
          std::filesystem::current_path(requested_path);
        } else {
          std::cout << "cd: " << requested_path << ": No such file or directory" << std::endl;
        }
      } else if (requested_path[0] == '.') {
          std::filesystem::path cwd = std::filesystem::current_path();
          std::string dir = cwd / requested_path;
          cwd = std::filesystem::canonical(dir);
          if (chdir(cwd.c_str()) == -1)
            std::cout << "cd: " << requested_path << ": No such file or directory" << std::endl;
          
        } else if (requested_path[0] = '~') {
          std::filesystem::current_path(home_string);
        }
      }
    else {
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
