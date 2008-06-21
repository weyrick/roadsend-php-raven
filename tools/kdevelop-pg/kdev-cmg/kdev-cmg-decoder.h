/* This file is part of kdev-cmg
   Copyright (C) 2005 Roberto Raggi <roberto@kdevelop.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KDEVCMG_DECODER_H
#define KDEVCMG_DECODER_H

#include "kdevcmg_parser.h"

#include <string>
#include <cstdlib>

namespace kdevcmg
{

class decoder
{
  parser::token_stream_type *_M_token_stream;

public:
  decoder(parser::token_stream_type *token_stream)
    : _M_token_stream(token_stream) {}

  int decode_op(std::size_t index) const
  {
    parser::token_type const &tk = _M_token_stream->token(index);
    return tk.kind;
  }

  std::string decode_id(std::size_t index) const
  {
    parser::token_type const &tk = _M_token_stream->token(index);
    return std::string(&tk.text[tk.begin], tk.end - tk.begin);
  }

  long decode_number(std::size_t index) const
  {
    parser::token_type const &tk = _M_token_stream->token(index);
    return ::strtol(&tk.text[tk.begin], 0, 0);
  }
};

} // end of namespace kdevcmg

#endif // DECODER_H
