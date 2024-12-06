#include <iostream>
#include <vector>
#include <sstream>

std::vector<std::string> split(const std::string& str, char delimeter){
  std::vector<std::string> tokens;
  std::stringstream ss(str);
  std::string token;

  while (std::getline(ss,token, delimeter)){
      tokens.push_back(token);
  }
  return tokens;
}

void commandInp(std::string input){


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
        std::cout << validTypes << ": not found" << std::endl; 
      }
    }

    else {
      std::cout << input << ": command not found" << std::endl;
    }
    std::cout << "$ ";
  }
  exit(0);
}
