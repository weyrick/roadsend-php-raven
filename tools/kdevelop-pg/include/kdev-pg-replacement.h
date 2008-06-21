/*
  This file is part of kdev-pg
  Copyright 2005, 2006, 2007 Roberto Raggi <roberto@kdevelop.org>

  Permission to use, copy, modify, distribute, and sell this software and its
  documentation for any purpose is hereby granted without fee, provided that
  the above copyright notice appear in all copies and that both that
  copyright notice and this permission notice appear in supporting
  documentation.

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  KDEVELOP TEAM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef KDEV_PG_REPLACEMENT_H
#define KDEV_PG_REPLACEMENT_H

#include <deque>
#include <string>

#include "kdev-pg-token-stream.h"

template <typename _String>
class default_replacement {
public:
  typedef _String string_type;
  
  inline default_replacement():
    _M_begin(0), _M_end(0) {}

  inline default_replacement(std::size_t begin, std::size_t end, string_type const &text):
    _M_begin(begin), _M_end(end), _M_text(text) {}

  std::size_t begin() const { return _M_begin; }
  std::size_t end() const { return _M_end; }
  string_type const &text() const { return _M_text; }

private:
  std::size_t _M_begin;
  std::size_t _M_end;
  string_type _M_text;
};

template <class _Replacement = default_replacement<std::string>,
	  class _TokenStream = kdev_pg_token_stream>
class replacement_list: public std::deque<_Replacement> {
  typedef std::deque<_Replacement> _Base;

public:
  typedef _Replacement replacement_type;
  typedef typename _Replacement::string_type string_type;
  typedef typename _Base::iterator iterator;
  typedef typename _Base::const_iterator const_iterator;

  using _Base::begin;
  using _Base::end;

public:
  replacement_list();
  ~replacement_list();

  _TokenStream *token_stream() const;
  void set_token_stream(_TokenStream *token_stream);

  template <class _Ast>
  void insert_text_before(_Ast *node, string_type const &text);

  template <class _Ast>
  void insert_text_after(_Ast *node, string_type const &text);

  template <class _Ast>
  void replace_text(_Ast *node, string_type const &text);

  void replace_text(std::size_t begin, std::size_t end, string_type const &text);

  template <class _Ast>
  void remove_text(_Ast *node);

  template <typename _OutputIterator>
  void operator()(string_type const &source, _OutputIterator out) const;

private:
  struct _Compare_replacement {
    bool operator()(_Replacement const &a, std::size_t pos) const {
      return a.begin() < pos;
    }
  };

private:
  _TokenStream *_M_token_stream;
};

template <class _Replacement, class _TokenStream>
replacement_list<_Replacement, _TokenStream>::replacement_list():
  _M_token_stream(0) {}

template <class _Replacement, class _TokenStream>
replacement_list<_Replacement, _TokenStream>::~replacement_list() {}

template <class _Replacement, class _TokenStream>
_TokenStream *replacement_list<_Replacement, _TokenStream>::token_stream() const {
  return _M_token_stream;
}

template <class _Replacement, class _TokenStream>
void replacement_list<_Replacement, _TokenStream>::set_token_stream(_TokenStream *token_stream) {
  _M_token_stream = token_stream; 
}

template <class _Replacement, class _TokenStream>
template <class _Ast>
void replacement_list<_Replacement, _TokenStream>::insert_text_before(_Ast *node,
								      string_type const &text) {
  std::size_t start_position = _M_token_stream->token(node->start_token).begin;
  replace_text(start_position, start_position, text);
}

template <class _Replacement, class _TokenStream>
template <class _Ast>
void replacement_list<_Replacement, _TokenStream>::insert_text_after(_Ast *node,
								     string_type const &text) {
  std::size_t end_position = _M_token_stream->token(node->end_token).begin;
  replace_text(end_position, end_position, text);
}

template <class _Replacement, class _TokenStream>
template <class _Ast>
void replacement_list<_Replacement, _TokenStream>::replace_text(_Ast *node,
								string_type const &text) {
  std::size_t start_position = _M_token_stream->token(node->start_token).begin;
  std::size_t end_position = _M_token_stream->token(node->end_token).begin;

  replace_text(start_position, end_position, text);
}

template <class _Replacement, class _TokenStream>
void replacement_list<_Replacement, _TokenStream>::replace_text(std::size_t first,
								std::size_t last,
								string_type const &text) {
  iterator __pos = std::lower_bound(begin(), end(), first, _Compare_replacement());
  insert(__pos, _Replacement(first, last, text));
}

template <class _Replacement, class _TokenStream>
template <class _Ast>
void replacement_list<_Replacement, _TokenStream>::remove_text(_Ast *node) {
  replace_text(node, string_type());
}

template <class _Replacement, class _TokenStream>
template <typename _OutputIterator>
void replacement_list<_Replacement, _TokenStream>::operator()(string_type const &source, _OutputIterator out) const {
  std::size_t pos = 0;

  for (const_iterator it = begin(); it != end(); ++it) {
    _Replacement const &repl = *it;

    std::copy(source.begin() + pos, source.begin() + (repl.begin() - pos), out);
    std::copy(repl.text().begin(), repl.text().end(), out);
    pos = repl.end();
  }
    
  std::copy(source.begin() + pos, source.end(), out);
}

#endif // KDEV_PG_REPLACEMENT_H
