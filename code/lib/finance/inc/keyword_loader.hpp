#pragma once

#include <string>
#include <map>

namespace finance {

class KeywordLoader {
public:
    // Constructor takes the path to the keyword mapping file
    explicit KeywordLoader(const std::string& filepath);
    
    // Load keywords from file and return as a map
    std::map<std::string, std::string> loadKeywords();
    
private:
    std::string filepath_;
};

} // namespace finance 