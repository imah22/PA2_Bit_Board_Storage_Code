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

#include <math.h>
#include <cstring>
#include "BitArray2D.hpp"

BitArray2D::BitArray2D(unsigned int rows, unsigned int columns)
 : rows(rows)
 , columns(columns) {
   const auto bits = rows * columns; // Number of bits
   const auto bytes = (bits+7)/8; // Round up the number of bytes (e.g. for 7 bits we need at least 1 byte)

   if(bytes == 0){
       throw BitArray2DException("Invalid size");
   }

   array = new char[bytes];
   memset(array, 0, bytes);
}


BitArray2D::~BitArray2D() {
   delete[] array;
}


bool BitArray2D::get(unsigned int row, unsigned int column){
   check_array_bounds(row, column);

   // get the element
   return get_bit_elem(array, columns, row, column);
}

void BitArray2D::set(unsigned int row, unsigned int column){
   check_array_bounds(row, column);

   // set the element
   set_bit_elem(array, columns, row, column);
}

void BitArray2D::check_array_bounds(unsigned int row, unsigned int column) const{
   if(row >= rows || column >= columns)
   {
       throw BitArray2DException("Invalid array indices");
   }
}
