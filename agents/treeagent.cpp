#include "include/treeagent.h"
#include "gaenari/gaenari.hpp"

treeagent::treeagent(int id, int num_players) : num_cards_((num_players <= 3) ? 5 : 4), id_(id), num_players_(num_players) {
    gaenari::logger::init1("../_log.txt");
    using supul_t = supul::supul::supul_t;
    supul_t::api::project::create("../supul_dir");

    // ADD FEATURES
    supul_t::api::project::add_field("../supul_dir", "x1", "REAL");
    supul_t::api::project::add_field("../supul_dir", "x2", "INTEGER");
    supul_t::api::project::add_field("../supul_dir", "x3", "TEXT_ID");
    supul_t::api::project::x("../supul_dir", {"x1", "x2", "x3"});

    // ADD LABEL
    supul_t::api::project::add_field("../supul_dir", "move", "TEXT_ID");
    supul_t::api::project::y("../supul_dir", "move");

    supul_t::api::project::set_property("../supul_dir", "db.type", "sqlite");
}

move treeagent::play(State s) {
    // represent s as the set of features
    // input those features in tree.predict() to get the move, and then return it
    return move(INVALID_MOVE);
}

void treeagent::observe(State s, move m) {

}

void treeagent::observe_before_move(State s) {

}

int treeagent::get_id() {
    return id_;
}

int treeagent::get_n_cards() {
    return num_cards_;
}

void treeagent::insert_and_update(std::string path_to_csv) {
    using supul_t = supul::supul::supul_t;
    supul_t supul;

    supul.api.lifetime.open("../supul_dir");
    supul.api.model.insert_chunk_csv(path_to_csv);
    supul.api.model.update();
    supul.api.lifetime.close();
}

void treeagent::rebuild() {
    using supul_t = supul::supul::supul_t;
    supul_t supul;

    supul.api.lifetime.open("../supul_dir");
    supul.api.model.rebuild();
    supul.api.lifetime.close();
}