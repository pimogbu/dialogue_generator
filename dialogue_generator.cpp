#include <iostream>
#include <map>
#include <string>
#include <sstream>

class JSONWriter {
private:
    std::stringstream ss;

    // Helper to escape special JSON characters
    std::string escape(const std::string& input) {
        std::string output;
        for (char c : input) {
            switch (c) {
                case '"': output += "\\\""; break;
                case '\\': output += "\\\\"; break;
                case '\b': output += "\\b"; break;
                case '\f': output += "\\f"; break;
                case '\n': output += "\\n"; break;
                case '\r': output += "\\r"; break;
                case '\t': output += "\\t"; break;
                default: output += c; break;
            }
        }
        return output;
    }

public:
    void generateJSON(const std::string& startText, const std::map<std::string, std::string>& optionTree, bool includeResponder = false, const std::string& responder = "") {
        ss.str(""); // Clear the stringstream
        ss << "{\n";
        
        // Conditionally add responder
        if (includeResponder) {
            ss << "  \"responder\": \"" << escape(responder) << "\",\n";
        }
        
        ss << "  \"start_text\": \"" << escape(startText) << "\",\n";
        ss << "  \"option_tree\": {\n";

        // Iterate through options
        bool first = true;
        for (const auto& pair : optionTree) {
            if (!first) ss << ",\n";
            ss << "    \"" << escape(pair.first) << "\": \"" << escape(pair.second) << "\"";
            first = false;
        }

        ss << "\n  }\n";
        ss << "}";
    }

    std::string getJSON() const {
        return ss.str();
    }
};

int main() {
    std::string responder;
    std::string startText;
    std::map<std::string, std::string> optionTree;
    bool includeResponder = false;

    // Ask if user wants to specify a responder
    std::string response;
    std::cout << "Do you want to add a responder? (yes/no): ";
    std::getline(std::cin, response);

    if (response == "yes" || response == "y") {
        std::cout << "Enter the responder: ";
        std::getline(std::cin, responder);
        includeResponder = true;
    }

    // Get start text
    std::cout << "Enter the start text for the dialogue: ";
    std::getline(std::cin, startText);

    // Get options and responses
    while (true) {
        std::string option;
        std::cout << "Enter an option (or press Enter to finish): ";
        std::getline(std::cin, option);

        if (option.empty()) {
            break;
        }

        std::string response;
        std::cout << "Enter the response for '" << option << "': ";
        std::getline(std::cin, response);

        // Ensure response ends with 'END'
        if (response.length() < 3 || response.substr(response.length() - 3) != "END") {
            response += "END";
        }

        optionTree[option] = response;
    }

    // Generate and print JSON
    JSONWriter jsonWriter;
    jsonWriter.generateJSON(startText, optionTree, includeResponder, responder);
    std::cout << "Generated Dialogue JSON:\n" 
              << jsonWriter.getJSON() << std::endl;

    return 0;
}