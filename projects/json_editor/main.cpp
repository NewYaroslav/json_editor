#include <nlohmann/json.hpp>
#include <string>
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <fstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;

void setBuff(char* buf, int sizeBuf, std::string text);

void showJsonNull(json& j);
bool addToJson(json& j);
bool checkFile(std::string fileName);

int main(int argc, char* argv[]) {
    cout << "Hello world!" << endl;
    json j;
    if (argc > 1) {
        // вывод второй строки из массива указателей на строки(нумерация в строках начинается с 0 )
        std::string path(argv[1]);
        cout << path << endl;
        std::ifstream i(path);
        if (!i.is_open()) {
            std::cout << "error! if (!i.is_open())" << std::endl;
            exit(0);
            return 0;
        }
        try {
            //j = json::parse(path);
            i >> j;
        }
        catch (json::parse_error& e) {
            // output exception information
            std::cout << "message: " << e.what() << '\n'
                  << "exception id: " << e.id << '\n'
                  << "byte position of error: " << e.byte << std::endl;
            j = nullptr;
        }
        i.close();
    } else {
        cout << "Not arguments" << endl;
    }

    sf::RenderWindow window(sf::VideoMode(1200, 780), "automatic exchange");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window, false);
    ImGui::StyleColorsDark();

    ImFontConfig font_config;
    font_config.OversampleH = 1; //or 2 is the same
    font_config.OversampleV = 1;
    font_config.PixelSnapH = 1;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x044F, // Cyrillic
        0,
    };
    ImGuiIO& io = ImGui::GetIO();
    if(checkFile("Roboto-Medium.ttf")) {
        io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 18.0f, NULL, ranges);
    } else
    if(checkFile("C:\\Windows\\Fonts\\Roboto-Medium.ttf")) {
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Roboto-Medium.ttf", 18.0f, NULL, ranges);
    } else
    if(checkFile("C:\\Windows\\Fonts\\ArialUni.ttf")) {
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, ranges);
    }

    ImGui::SFML::UpdateFontTexture();

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        showJsonNull(j);
        if(j.is_null() || j.empty()) {
            //showJsonNull(j);
        } else {
            //showJsonNull(j);
        }

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    system("pause");
    return 0;
}

void showJsonNull(json& j) {
    ImGui::Begin("json");

    if(j.is_null()) {
        ImGui::Text("null");
    } else
    if(j.empty()) {
        ImGui::Text("empty");
    } else
    if(j.is_array()) {
        // special iterator member functions for objects
        for (json::iterator it = j.begin(); it != j.end(); ++it) {
            std::cout << it.key() << " : " << it.value() << "\n";
            //ImGui::Text("empty");
        }
    } else {
        //ImGui::Separator("json");
        //ImGui::TreeNode()
        //ImGui::Columns( j.size(), "mycolumns" );
        //for (json::iterator it = j.begin(); it != j.end(); ++it) {
        for (json::iterator it = j.begin(); it != j.end(); ++it) {
            //std::cout << it.key() << " : " << it.value() << "\n";
            json j_data = j[it.key()];

            const int TEXT_SIZE = 1024;
            char text_key[TEXT_SIZE];
            char text_val[TEXT_SIZE];
            //std::cout << j_data << std::endl;
            if(j_data.is_null()) {
                std::string key(it.key());
                setBuff(text_key, TEXT_SIZE, key);
                ImGui::InputText("##key", text_key, TEXT_SIZE, ImGuiInputTextFlags_None);
            } else
            if(j_data.is_array()) {
                std::string key(it.key());
                std::string data;
                for(int i = 0; i < j[key].size(); ++i) {
                    std::string val = j[key][i].dump();
                    data += val + "\n";
                }
                ImGui::InputTextMultiline(key.c_str(), (char*)data.c_str(), data.size(), ImVec2(128, 64), ImGuiInputTextFlags_AutoSelectAll);
            } else {
            //if(j_data.is_string()) {
                std::string key = it.key();
                std::string val = j[key].dump();
                setBuff(text_key, TEXT_SIZE, key);
                setBuff(text_val, TEXT_SIZE, val);
                ImGui::InputText("##key", text_key, TEXT_SIZE, ImGuiInputTextFlags_None);
                ImGui::SameLine();
                ImGui::InputText("##val", text_val, TEXT_SIZE, ImGuiInputTextFlags_None);
            }

            /*
            std::cout << *it << '\n';
            std::cout << it.key() << " : " << it.value() << "\n";
            std::string key(it.key());
            //ImGui::Text("empty");
            //ImGui::Selectable(key.c_str());

            //ImGui::Text(key.c_str());
            char text[1024]; setBuff(text, 1024, key);
            ImGui::InputText("##key", text, 1024, ImGuiInputTextFlags_None);
            std::string newKey(text);
            if(newKey != key) {
                json j_copy = j[key];
                j[newKey] = j_copy;
                j.erase(key);
                break;
            }
            */
            //ImGui::NextColumn();
        }
    }


    static bool isAdd = false;
    if(isAdd) {
        //if(addToJson(j)) isAdd = false;
        ImGui::Begin("add to JSON");
        static int type_details_item_current = 0;
        const char* type_details_items[] = { "null", "boolean", "double", "int", "object", "array", "string"};
        ImGui::ListBox("type", &type_details_item_current, type_details_items, IM_ARRAYSIZE(type_details_items), 7);

        static char str_key[1024] = "key";
        static double d_number = 0;
        static int i_number = 0;
        static int bool_details_item_current = 0;
        const char* bool_details_items[] = { "false", "true"};

        switch (type_details_item_current) {
            case 0:
                ImGui::InputText("key", str_key, 1024, ImGuiInputTextFlags_AutoSelectAll);
                if(ImGui::Button("ok")) {
                    std::string key(str_key);
                    j[key] = nullptr;
                    isAdd = false;
                }
            break;
            case 1:
                ImGui::InputText("key", str_key, 1024, ImGuiInputTextFlags_AutoSelectAll);
                ImGui::ListBox("bool is", &bool_details_item_current, bool_details_items, IM_ARRAYSIZE(bool_details_items), 2);
                if(ImGui::Button("ok")) {
                    std::string key(str_key);
                    if(bool_details_item_current == 1) j[key] = true;
                    else j[key] = false;
                    isAdd = false;
                }
            break;
            case 2:
                ImGui::InputText("key", str_key, 1024, ImGuiInputTextFlags_AutoSelectAll);
                ImGui::InputDouble("number", &d_number, 0.01f, 1.0f, "%.15d");
                if(ImGui::Button("ok")) {
                    std::string key(str_key);
                    j[key] = d_number;
                    isAdd = false;
                }
            break;
            case 3:
                ImGui::InputText("key", str_key, 1024, ImGuiInputTextFlags_AutoSelectAll);
                ImGui::InputInt("number", &i_number, 1, 1);
                if(ImGui::Button("ok")) {
                    std::string key(str_key);
                    j[key] = d_number;
                    isAdd = false;
                }
            break;
            case 4:
            break;
            case 5:
            break;
            case 6:
            break;
        }
        ImGui::End();
    }
    if(ImGui::Button("add")) {
        isAdd = true;
    }
    ImGui::End();
}

bool addToJson(json& j) {
    ImGui::Begin("add to JSON");

    static int type_details_item_current = 0;
    const char* type_details_items[] = { "null", "boolean", "double", "int", "object", "array", "string"};
    ImGui::ListBox("type", &type_details_item_current, type_details_items, IM_ARRAYSIZE(type_details_items), 7);

    static char str_key[1024] = "key";
    static double d_number = 0;
    static int i_number = 0;
    static int bool_details_item_current = 0;
    const char* bool_details_items[] = { "false", "true"};

    switch (type_details_item_current) {
        case 0:
            ImGui::InputText("key", str_key, 1024, ImGuiInputTextFlags_AutoSelectAll);
            if(ImGui::Button("ok")) {
                std::string key(str_key);
                j[key] = nullptr;
                return true;
            }
        break;
        case 1:
            ImGui::InputText("key", str_key, 1024, ImGuiInputTextFlags_AutoSelectAll);
            ImGui::ListBox("bool is", &bool_details_item_current, bool_details_items, IM_ARRAYSIZE(bool_details_items), 2);
            if(ImGui::Button("ok")) {
                std::string key(str_key);
                if(bool_details_item_current == 1) j[key] = true;
                else j[key] = false;
                return true;
            }
        break;
        case 2:
            ImGui::InputText("key", str_key, 1024, ImGuiInputTextFlags_AutoSelectAll);
            ImGui::InputDouble("number", &d_number, 0.01f, 1.0f, "%.15d");
            if(ImGui::Button("ok")) {
                std::string key(str_key);
                j[key] = d_number;
                return true;
            }
        break;
        case 3:
            ImGui::InputText("key", str_key, 1024, ImGuiInputTextFlags_AutoSelectAll);
            ImGui::InputInt("number", &i_number, 1, 1);
            if(ImGui::Button("ok")) {
                std::string key(str_key);
                j[key] = d_number;
                return true;
            }
        break;
        case 4:
        break;
        case 5:
        break;
        case 6:
        break;
    }
    ImGui::End();
    return false;
}

void setBuff(char* buf, int sizeBuf, std::string text) {
    memset(buf, 0, sizeBuf);
    for(int i = 0; i < text.size(); ++i) {
        buf[i] = text[i];
    }
}

bool checkFile(std::string fileName) {
   std::ifstream file;
   file.open(fileName);
   if(!file) return false;
   file.close();
   return true;
}
