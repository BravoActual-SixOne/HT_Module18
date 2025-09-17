#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>   // <--- доступно по юниту
namespace fs = std::filesystem;

class User {
    std::string _name;
    std::string _login;
    std::string _pass;
public:
    User() {}
    User(const std::string& n, const std::string& l, const std::string& p)
        : _name(n), _login(l), _pass(p) {
    }

    std::string serialize() const {
        return _name + ";" + _login + ";" + _pass;
    }
    static User deserialize(const std::string& line) {
        size_t p1 = line.find(';');
        size_t p2 = line.find(';', p1 + 1);
        return User(
            line.substr(0, p1),
            line.substr(p1 + 1, p2 - p1 - 1),
            line.substr(p2 + 1)
        );
    }
    void print() const {
        std::cout << "User: " << _name << " (" << _login << ")\n";
    }
};

class Message {
    std::string _text;
    std::string _sender;
    std::string _receiver;
public:
    Message() {}
    Message(const std::string& t, const std::string& s, const std::string& r)
        : _text(t), _sender(s), _receiver(r) {
    }

    std::string serialize() const {
        return _text + ";" + _sender + ";" + _receiver;
    }
    static Message deserialize(const std::string& line) {
        size_t p1 = line.find(';');
        size_t p2 = line.find(';', p1 + 1);
        return Message(
            line.substr(0, p1),
            line.substr(p1 + 1, p2 - p1 - 1),
            line.substr(p2 + 1)
        );
    }
    void print() const {
        std::cout << "[" << _sender << " -> " << _receiver << "]: " << _text << "\n";
    }
};

void saveUsers(const std::vector<User>& users, const std::string& filename) {
    std::ofstream ofs(filename);
    for (auto& u : users) ofs << u.serialize() << "\n";
    ofs.close();
    // права: только для владельца rw-
    fs::permissions(filename,
        fs::perms::owner_read | fs::perms::owner_write,
        fs::perm_options::replace);
}

std::vector<User> loadUsers(const std::string& filename) {
    std::vector<User> res;
    std::ifstream ifs(filename);
    std::string line;
    while (std::getline(ifs, line)) res.push_back(User::deserialize(line));
    return res;
}

void saveMessages(const std::vector<Message>& msgs, const std::string& filename) {
    std::ofstream ofs(filename);
    for (auto& m : msgs) ofs << m.serialize() << "\n";
    ofs.close();
    fs::permissions(filename,
        fs::perms::owner_read | fs::perms::owner_write,
        fs::perm_options::replace);
}

std::vector<Message> loadMessages(const std::string& filename) {
    std::vector<Message> res;
    std::ifstream ifs(filename);
    std::string line;
    while (std::getline(ifs, line)) res.push_back(Message::deserialize(line));
    return res;
}

int main() {
    std::vector<User> users = {
        User("Alice", "alice", "1234"),
        User("Bob", "bob", "qwerty")
    };
    std::vector<Message> messages = {
        Message("Привет!", "Alice", "Bob"),
        Message("Дароу!", "Bob", "Alice")
    };

    saveUsers(users, "users.db");
    saveMessages(messages, "messages.db");

    auto loadedUsers = loadUsers("users.db");
    auto loadedMessages = loadMessages("messages.db");

    std::cout << "📂 Прочитали пользователей и сообщения из файлов:\n";
    for (auto& u : loadedUsers) u.print();
    for (auto& m : loadedMessages) m.print();

    return 0;
}
