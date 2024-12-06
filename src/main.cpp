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
  std::string input;
  while(true){
    std::cout << "$ ";
    std::getline(std::cin, input);
    if(input == "exit 0"){
      return 0;
    }
    std::cout << input << ": command not found" << std::endl;
  }
  return 0;
}
