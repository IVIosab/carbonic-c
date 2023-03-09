#include <iostream>
#include <unordered_map>
#include <stack>
#define first f
#define second s

std::unordered_map <std::string, bool> variableNames;
// pair < variable name, scope number > 
// -> when a new scope is introduced, its id is (lastScopeId + 1).
std::stack <std::pair<std::string, int>> variableScope;