
/*
 * File: scanner.h
 * Provides tokens for parser as a lexer.
 */

/* PEL literals.
 * Integer    ([1-9][0-9]*) | (0[0-7]*) | (0[xX][0-9a-fA-F]+)
 * Float      [0-9]*\.?([0-9]+([eE][-+]?[0-9]+)?)?
 * String     (\" ([^"\\]|\\.)* \") | (\' ([^'\\]|\\.)* \')
 * Ident      [_a-zA-Z][_a-zA-Z0-9]+
 * Operators  > < == <= >=  != =~ && || . , : ; [ ] ( ) { } = + += -= *
 *            null
 */

#ifndef PEL_SCANNER_H
#define PEL_SCANNER_H

#include<cstddef>
#include<cstdio>

namespace pel{

class Reader; // bufio.h
class Token; // token.h

class Scanner{
public:
  Scanner();

  ~Scanner();

  // Close opened file or stream.
  Scanner& Close();

  // Primary function. If @tok is 0, don't forget to free returned tokens,
  // as the scanner doesn't manager allocated tokens.
  // @tok If not 0, the next token will be placed into this address.
  // @return Next token of scanner.
  const Token* NextToken(Token *tok=0);

  // Open a file to scan.
  // @file File name.
  // @ok If not equals 0, filled with true when file opened successfully or false.
  // @return self.
  Scanner& Open(const char *file, bool *ok=0);

  // @return opened file name, not available if is from file stream or string.
  const char* File() const;

  // @return Source string.
  const char* From() const;

  // Scan from a file stream.
  Scanner& From(FILE* stream);

  // Scan from a string.
  // @string Please keep string alive during scanning.
  // @len Length of char array, if equals to 0 then treat string as a C string.
  Scanner& From(const char *string, std::size_t len=0);
  
  // @return
  unsigned int TabWidth() const;

  // Setter
  Scanner& TabWidth(unsigned int width);

private:
  FILE        *fin;   // file stream
  char        *fname; // file name
  const char  *string;// string source
  Reader      *reader;// buffer reader

  unsigned int tabwidth; // default is 4
  std::size_t  line;  // line number of token in source
  std::size_t  col;   // column number of token in source

private:
  Scanner(const Scanner&); // copy is disabled
  Scanner& operator=(const Scanner&);
};

} // ~pel

#endif // ~PEL_SCANNER_H