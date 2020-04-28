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
#include "Client.hpp"

Client::~Client() {
    if(initialized)
    {
        std::string filename("player_" + std::to_string(player) + ".action_board.json");
        remove(filename.c_str());
    }
}


void Client::initialize(unsigned int player, unsigned int board_size){
    if(player < 1 || player > MAX_PLAYERS)
    {
        throw ClientWrongPlayerNumberException();
    }

    std::vector<int> row(board_size, 0);
    std::vector<std::vector<int>> board(board_size, row);

    std::ofstream filename("player_" + std::to_string(player) + ".action_board.json");
    cereal::JSONOutputArchive json_output_archive(filename);
    json_output_archive(CEREAL_NVP(board));

    this->player = player;
    this->board_size = board_size;
    this->initialized = true;
}


void Client::fire(unsigned int x, unsigned int y) {
    std::ofstream shot_file("player_" + std::to_string(player) + ".shot.json");
    cereal::JSONOutputArchive json_output_archive(shot_file);
    json_output_archive(CEREAL_NVP(x), CEREAL_NVP(y));
}


bool Client::result_available() {
    std::ifstream result_file("player_" + std::to_string(player) + ".result.json");
    return result_file.good();
}


int Client::get_result() {
    std::string result_filename("player_" + std::to_string(player) + ".result.json");
    std::ifstream result_file(result_filename);
    cereal::JSONInputArchive json_input_archive(result_file);
    int result;
    json_input_archive(CEREAL_NVP(result));

    if(result != HIT && result != MISS && result != OUT_OF_BOUNDS)
    {
        throw ClientException("Invalid result value");
    }

    remove(result_filename.c_str());

    return result;
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    std::string filename("player_" + std::to_string(player) + ".action_board.json");
    std::ifstream action_file_in(filename);
    cereal::JSONInputArchive json_input_archive(action_file_in);
    std::vector<std::vector<int>> board;
    json_input_archive(CEREAL_NVP(board));

    board[y][x] = result;

    std::ofstream action_file_out(filename);
    cereal::JSONOutputArchive json_output_archive(action_file_out);
    json_output_archive(CEREAL_NVP(board));
}


string Client::render_action_board(){
    std::string filename("player_" + std::to_string(player) + ".action_board.json");
    std::ifstream action_file_in(filename);
    cereal::JSONInputArchive json_input_archive(action_file_in);
    std::vector<std::vector<int>> board;
    json_input_archive(CEREAL_NVP(board));

    std::string ascii_board;
    for(size_t i = 0; i < board.size(); ++i)
    {
        for(size_t j = 0; j < board[i].size(); ++j)
        {
            const int value = board[i][j];
            if(value == HIT)
            {
                ascii_board += 'X';
            }
            else if(value == MISS)
            {
                ascii_board += '0';
            }
            else
            {
                ascii_board += '-';
            }
        }

        ascii_board += '\n';
    }

    return ascii_board;
}
