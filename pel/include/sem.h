
/*
 * File: sem.h
 * Provides sematic analysis by travaling AST.
 */

/*
 *
 *
 */

#ifndef PEL_SEM_H
#define PEL_SEM_H

namespace pel
{

class ASTree; // ast.h


// Analyze result info.
class Analysis{

public:
  Analysis();

  Analysis(const Analysis&);

  ~Analysis();

  Analysis& operator=(const Analysis&);

private:

};


// Main class.
class Analyzer{

public:
  Analyzer();

  ~Analyzer();
  
  //
  // Analyze an AST and return its result.
  // @tree  AST.
  // @result If not 0, the result will be placed into this address.
  // @return New result, don't forget to destroy result if @result is 0.
  Analysis* Analyze(const ASTree *tree, Analysis *result=0);

private:
  const ASTree *tree;

private:
  Analyzer(const Analyzer&);  // copying a analyzer is meaningless.
  Analyzer& operator=(const Analyzer&);
  

};


} // ~pel


#endif
