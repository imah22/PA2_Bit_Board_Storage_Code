//
// Created by Ibrahim on 4/16/2020.
//
//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "common.hpp"
#include "Server.hpp"


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
    int read_pos = file->tellg();
    int length = 0;

    file->seekg(0, ios_base::beg);
    std::string line;
    while(std::getline(*file, line))
    {
        length += line.size();
    }

    file->clear();
    file->seekg(read_pos, ios_base::beg);

    return length;
}

void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    // First reset the pointers
    delete this->p1_setup_board;
    delete this->p2_setup_board;
    this->p1_setup_board = nullptr;
    this->p2_setup_board = nullptr;

    this->board_size = board_size;
    this->p1_setup_board = scan_setup_board(p1_setup_board);
    this->p2_setup_board = scan_setup_board(p2_setup_board);
}


Server::~Server() {
    delete p1_setup_board;
    delete p2_setup_board;
}


BitArray2D *Server::scan_setup_board(string setup_board_name){
    auto file = ifstream(setup_board_name);
    if(!file.good())
    {
        throw ServerException("Failed to open board");
    }

    const int expected_length = board_size * board_size;
    if(get_file_length(&file) != expected_length)
    {
        throw ServerException("Invalid board size");
    }

    BitArray2D* bit_array = new BitArray2D(board_size, board_size);
    for(auto i = 0; i < board_size; ++i){
        std::string line;
        std::getline(file, line);
        for(auto j = 0; j < board_size; ++j){
            if(line[j] != '_') {
                bit_array->set(i, j);
            }
        }
    }

    return bit_array;
}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if(player < 1 || player > MAX_PLAYERS)
    {
        throw ServerException("Invalid player id");
    }

    if(x >= board_size || y >= board_size)
    {
        return OUT_OF_BOUNDS;
    }

    // The array is the board of the opponent player
    BitArray2D* array = player == 1 ? p2_setup_board : p1_setup_board;
    if(array->get(y, x))
    {
        return HIT;
    }

    return MISS;
}


int Server::process_shot(unsigned int player) {
    if(player < 1 || player > MAX_PLAYERS)
    {
        throw ServerException("Invalid player id");
    }

    const std::string shot_filename = "player_" + std::to_string(player) + ".shot.json";
    std::ifstream shot_file(shot_filename);
    if(!shot_file.good())
    {
        return NO_SHOT_FILE;
    }

    cereal::JSONInputArchive json_input_archive(shot_file);
    int x,y;
    json_input_archive(x, y);

    int result = evaluate_shot(player, x, y);

    std::ofstream result_file("player_" + std::to_string(player) + ".result.json");
    cereal::JSONOutputArchive json_output_archive(result_file);
    json_output_archive(CEREAL_NVP(result));

    remove(shot_filename.c_str());

    if(result == OUT_OF_BOUNDS)
    {
        return NO_SHOT_FILE;
    }

    return SHOT_FILE_PROCESSED;
}

