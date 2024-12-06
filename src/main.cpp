#include <iostream>

void invalidCommand(std::string input){
  std::cout << "$ ";
  std::getline(std::cin, input);
  std::cout << input << ": command not found" << std::endl;

}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage  
  std::string input;
  while(true){
    invalidCommand(input);
  }
  return 0;
}
