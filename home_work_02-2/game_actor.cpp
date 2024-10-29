//
// Created by flyon21 on 26.10.24.
//
#include <iostream>
#include <utility>
#include <vector>
#include <string>

int counter = 0;
int pointer_counter = 0;

class GameActor {
public:
    std::string name;

    GameActor() : name("Default Actor") {
        counter++;
    }

    explicit GameActor(std::string name) : name(std::move(name)) {
        counter++;
    }

    void draw() const {
        std::cout << name << std::endl;
    }
};

std::vector<GameActor *> actors;

GameActor *duplicateGameActor(const size_t index) {
    GameActor *pointer = nullptr;
    if (index < actors.size() && actors[index] != nullptr) {
        pointer = new GameActor(*actors[index]);
        pointer_counter++;
        return pointer;
    } else {
        return new GameActor();
    }
}

GameActor *duplicateGameActor(GameActor *ptr) {
    GameActor *pointer = nullptr;
    if (ptr != nullptr) {
        pointer = new GameActor(*ptr);
        pointer_counter++;
        return pointer;
    } else {
        return new GameActor();
    }
}

void clean() {
    int delete_counter = 0;
    for (GameActor *&actor: actors) {
        delete actor;
        actor = nullptr;
        delete_counter++;
    }
    actors.clear();
    std::cout << "Deleted " << delete_counter << " actors" << std::endl;
}

int main() {

    actors.push_back(new GameActor("Actor_1"));
    actors.push_back(new GameActor("Actor_2"));
    actors.push_back(new GameActor("Actor_3"));
    actors.push_back(new GameActor());

    actors.emplace_back(duplicateGameActor(static_cast<GameActor *>(nullptr)));
    actors.emplace_back(duplicateGameActor(static_cast<size_t>(2)));

    actors.emplace_back(duplicateGameActor(static_cast<GameActor *>(actors[1])));
    actors.emplace_back(duplicateGameActor(static_cast<GameActor *>(nullptr)));

    for (const auto &actor: actors) {
        actor->draw();
    }
    std::cout << "Created + Pointed: " << counter + pointer_counter << std::endl;

    clean();

    return 0;
}
