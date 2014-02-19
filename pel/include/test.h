/*
 * File: test.h
 * Provides common functions for testing.
 */


#ifndef PEL_TEST_H
#define PEL_TEST_H

namespace pel{

// called before test
void TestBegin(const char* label);

// called after test
void TestEnd();

}


#endif