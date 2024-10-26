//
// Created by flyon21 on 26.10.24.
//
#include <iostream>
#include <utility>
#include <vector>
#include <string>

class GameActor {
public:
    std::string name;

    GameActor() : name("Default Actor") {}
    explicit GameActor (std::string name) : name(std::move(name)) {}
    void draw() const {
        std::cout << name << std::endl;
    }
};

std::vector<GameActor*> actors;


GameActor* duplicateGameActor(const size_t index) {
    return index < actors.size() && actors[index] != nullptr ? new GameActor(*actors[index]) : new GameActor();
}

GameActor* duplicateGameActor(GameActor* ptr) {
    return ptr != nullptr ? new GameActor(*ptr) : new GameActor();
}

void clean() {
    for (GameActor* actor : actors) {
        delete actor;
        actor = nullptr;
    }
    actors.clear();
}

int main() {

    actors.push_back(new GameActor("Actor_1"));
    actors.push_back(new GameActor("Actor_2"));
    actors.push_back(new GameActor("Actor_3"));
    actors.push_back(new GameActor());

    actors.emplace_back(duplicateGameActor(nullptr));
    actors.emplace_back(duplicateGameActor(2));

    actors.emplace_back(duplicateGameActor(static_cast<GameActor*>(actors[1])));
    actors.emplace_back(duplicateGameActor(static_cast<GameActor*>(nullptr)));

    for (const auto &actor : actors) {
        actor->draw();
    }

    clean();

    return 0;
}
