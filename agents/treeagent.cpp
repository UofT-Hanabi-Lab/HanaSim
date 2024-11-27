#include <fstream>
#include <sstream>
#include <unordered_map>

#include "include/treeagent.h"
#include "../bots/include/holmesbot.h"
#include "../bots/include/smartbot.h"
#include "gaenari/gaenari.hpp"

treeagent::treeagent(int id, int num_players, std::string sample_path, player *partner) : num_cards_((num_players <= 3) ? 5 : 4), id_(id), num_players_(num_players), partner_(partner) {
    gaenari::logger::init1("../_log.txt");
    using supul_t = supul::supul::supul_t;
    supul_t::api::project::create("../supul_dir");
    std::ifstream sample_file(sample_path);

    // ADD FEATURES
    std::string first_line;
    std::getline(sample_file, first_line);
    std::istringstream iss(first_line);
    std::string feat;
    std::vector<std::string> in_feats = {};

    while(std::getline(iss, feat)) {
        if (feat == "move") {
            supul_t::api::project::add_field("../supul_dir", feat, "TEXT_ID");
        } else {
            supul_t::api::project::add_field("../supul_dir", feat, "INTEGER");
            in_feats.push_back(feat);
        }
    }

    supul_t::api::project::x("../supul_dir", in_feats);
    supul_t::api::project::y("../supul_dir", "move");

    supul_t::api::project::set_property("../supul_dir", "db.type", "sqlite");
}

move treeagent::play(State s) {
    // represent s as the set of features
    // input those features in tree.predict() to get the move, and then return it
    int hints = s.get_num_hints(); // feature
    int deck_count = s.get_deck().size(); // feature
    std::vector<int> piles = s.get_piles(); // feature

    std::vector<Card> discards = s.get_discards();
    std::vector<int> discard_count = {}; // feature
    discard_count.resize(25);
    for (Card c : discards) {
        discard_count[((int)c.color() - 1)*5 + ((int)c.rank() - 1)]++;
    }

    std::vector<std::vector<Card>> hands = s.get_hands();
    std::vector<int> hand_cols = {}; // feature
    std::vector<int> hand_ranks = {}; // feature
    for (int i = 0; i < num_players_; i++) {
        if (i == id_) {
            for (int j = 0; j < num_cards_; j++) {
                hand_cols.push_back(6);
                hand_ranks.push_back(6);
            }
        } else {
            for (int j = 0; j < num_cards_; j++) {
                hand_cols.push_back((int)(hands[i][j].color()));
                hand_ranks.push_back((int)(hands[i][j].rank()));
            }
        }
    }

    //HANDKNOWLEDGE
    std::vector<int> hk_playable = {};
    std::vector<int> hk_valuable = {};
    std::vector<int> hk_worthless = {};
    std::vector<int> hk_cols = {};
    std::vector<int> hk_ranks = {};


    if (dynamic_cast<smartbot*>(partner_) != nullptr) {
        smartbot *smart_partner = (smartbot*)(partner_);
        std::vector<std::vector<SmartBotInternal::cardknowledge>> hk = smart_partner->get_hk();
        for (int i = 0; i < hk.size(); i++) {
            for (int j = 0; j < hk[i].size(); j++) {
                hk_playable.push_back((hk[i][j].playable(s) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_playable() == SmartBotInternal::YES) ? 1 : 2));
                hk_valuable.push_back((hk[i][j].valuable(s) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_valuable() == SmartBotInternal::YES) ? 1 : 2));
                hk_worthless.push_back((hk[i][j].worthless(s) == SmartBotInternal::NO) ? 0 : ((hk[i][j].get_worthless() == SmartBotInternal::YES) ? 1 : 2));
                hk_cols.push_back((hk[i][j].color() < 0) ? 6 : hk[i][j].color());
                hk_cols.push_back((hk[i][j].rank() < 0) ? 6 : hk[i][j].rank());
            }
        }

    } else if (dynamic_cast<holmesbot*>(partner_) != nullptr) {
        holmesbot *holmes_partner = (holmesbot*)(partner_);
        std::vector<std::vector<HolmesBotInternal::cardknowledge>> hk = holmes_partner->get_hk();
        for (int i = 0; i < hk.size(); i++) {
            for (int j = 0; j < hk[i].size(); j++) {
                hk_playable.push_back((int)hk[i][j].is_playable);
                hk_valuable.push_back((int)hk[i][j].is_valuable);
                hk_worthless.push_back((int)hk[i][j].is_worthless);
                hk_cols.push_back((int)(hk[i][j].color()));
                hk_cols.push_back((int)(hk[i][j].rank()));
            }
        }
    }
    std::unordered_map<std::string, std::string> feats;
    feats = {{"deck_count", std::to_string(deck_count)},
             {"hints", std::to_string(hints)}};
    for (int i = 0; i < hand_cols.size(); i++) {
        feats.insert({"hand" + std::to_string((i+1) / (num_cards_+1)) + "_col" + std::to_string((i+1) % (num_cards_+1)), std::to_string(hand_cols[i])});
        feats.insert({"hand" + std::to_string((i+1) / (num_cards_+1)) + "_rank" + std::to_string((i+1) % (num_cards_+1)), std::to_string(hand_ranks[i])});
        feats.insert({"hk" + std::to_string((i+1) / (num_cards_+1)) + "_playable" + std::to_string((i+1) % (num_cards_+1)), std::to_string(hk_playable[i])});
        feats.insert({"hk" + std::to_string((i+1) / (num_cards_+1)) + "_valuable" + std::to_string((i+1) % (num_cards_+1)), std::to_string(hk_valuable[i])});
        feats.insert({"hk" + std::to_string((i+1) / (num_cards_+1)) + "_worthless" + std::to_string((i+1) % (num_cards_+1)), std::to_string(hk_worthless[i])});
        feats.insert({"hk" + std::to_string((i+1) / (num_cards_+1)) + "_col" + std::to_string((i+1) % (num_cards_+1)), std::to_string(hk_cols[i])});
        feats.insert({"hk" + std::to_string((i+1) / (num_cards_+1)) + "_rank" + std::to_string((i+1) % (num_cards_+1)), std::to_string(hk_ranks[i])});
    }
    for (int i = 0; i < piles.size(); i++) feats.insert({"pile"+std::to_string(i), std::to_string(piles[i])});
    for (int i = 0; i < discard_count.size(); i++) feats.insert({"discard"+std::to_string(((i+1) % 6) + ((i+1) / 6)), std::to_string(discard_count[i])});
    using supul_t = supul::supul::supul_t;
    supul_t supul;

    supul.api.lifetime.open("../supul_dir");
    auto ret = supul.api.model.predict(feats);
    if (ret.error) {
        std::cout << "uh oh" << std::endl;
        return move(INVALID_MOVE);
    }
    std::string move_str = ret.label;
    
    move_type movetype = (move_type)move_str[0];

    if (movetype == PLAY || movetype == DISCARD) {
        int move_index = (int)move_str[2];
        supul.api.lifetime.close();
        return move(movetype, id_, move_index);
    } else {
        int move_to = (int)move_str[2];
        std::vector<int> card_indices = {};
        if (movetype == RANK_HINT) {
            Rank r = (Rank)move_str[move_str.size() - 2];
            for (int i = 0; i < s.get_hands()[move_to].size(); i++) {
                if (s.get_hands()[move_to][i].rank() == r) card_indices.push_back(i);
            }
            supul.api.lifetime.close();
            return move(movetype, move_to, id_, card_indices, r);
        } else {
            Color c = (Color)move_str[move_str.size() - 2];
            for (int i = 0; i < s.get_hands()[move_to].size(); i++) {
                if (s.get_hands()[move_to][i].color() == c) card_indices.push_back(i);
            }
            supul.api.lifetime.close();
            return move(movetype, move_to, id_, card_indices, c);
        }
        
    }
    
    // Shouldn't reach here
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