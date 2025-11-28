#include "crow_all.h"
#include <string>
#include <iostream>

using namespace std;

// --- Your Core Logic (Refactored to return strings) ---

string encryptLogic(string text, int key) {
    for (size_t i = 0; i < text.length(); i++) {
        text[i] = text[i] + key;
    }
    return text;
}

string decryptLogic(string text, int key) {
    for (size_t i = 0; i < text.length(); i++) {
        text[i] = text[i] - key;
    }
    return text;
}

// --- Main Web Server ---

int main() {
    crow::SimpleApp app;

    // Route to serve the HTML page (Frontend)
    CROW_ROUTE(app, "/")([](){
        crow::mustache::context ctx;
        // Requires templates/index.html to exist
        return crow::mustache::load("index.html").render();
    });

    // Route to handle Encryption/Decryption requests
    CROW_ROUTE(app, "/process").methods("POST"_method)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        
        if (!x) return crow::response(400); // Bad Request if JSON is invalid

        string text = x["text"].s();
        int key = x["key"].i();
        string mode = x["mode"].s();
        string resultText;

        if (mode == "encrypt") {
            resultText = encryptLogic(text, key);
        } else {
            resultText = decryptLogic(text, key);
        }

        crow::json::wvalue result;
        result["result"] = resultText;
        
        return crow::response(result);
    });

    // Need to explicitly serve static files (CSS/JS) in C++
    // This tells the server: "If URL starts with /static/, look in the static folder"
    CROW_ROUTE(app, "/static/<string>")
    ([](string filename){
        std::ifstream in("static/" + filename, std::ifstream::in);
        if(in){
            std::ostringstream contents;
            contents << in.rdbuf();
            in.close();
            return crow::response(contents.str());
        } else {
            return crow::response(404);
        }
    });

    // Run the server on port 18080 (standard for Crow)
    cout << "Server running at http://localhost:18080" << endl;
    app.port(18080).multithreaded().run();
}