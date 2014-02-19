
#include<cassert>
#include<cstdio>
#include"ast.h"
#include"map.h"
#include"new.h"
#include"sem.h"

using namespace pel;


//----------------------------------
//  Analysis impl
//-----------------------------------
Analysis::Analysis()
{

}

Analysis::Analysis(const Analysis& a)
{

}

Analysis& Analysis::operator=(const Analysis& a)
{
  if( this != &a ){
  }
  return *this;
}

Analysis::~Analysis()
{

}



//------------------------------------
// Analyzer impl
//------------------------------------
Analyzer::Analyzer()
{

}

Analyzer::~Analyzer()
{

}


Analysis* Analyzer::Analyze(const ASTree *tree, Analysis *result)
{
  assert(tree);

  this->tree = tree;

  if( result == 0 ){
    result = new Analysis();
  }

  

  return result;
}



//------------------------------------
// Test Analyzer
//------------------------------------

#include "test.h"

namespace pel{

void TestAnalyzer()
{


}


} // ~pel
