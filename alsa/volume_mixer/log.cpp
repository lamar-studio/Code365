/*
 * Copyright (c) 2015 Josh Williams <theprime@codingprime.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the COPYING file for details.
 */
#include <iostream>

#include "log.h"

namespace VOsd {

	Log::Log() :
		mType(LogTypeNormal) {
	}

	Log::~Log(void) {
		if (mType == LogTypeError) {
			std::cout << mOutputStream.str() << std::endl;
		} else {
			std::cout << mOutputStream.str() << std::endl;
		}
	}

	Log& Log::Debug(void) {
		mType = LogTypeNormal;
		return *this;
	}

	Log& Log::Error(void) {
		mType = LogTypeError;
		return *this;
	}

	/* template<typename T> */
	/* Log& Log::operator <<(T pVal) { */
	/* 	mOutputStream << pVal; */
	/* 	return *this; */
	/* } */

	/* Log& Log::operator <<(const char* pMsg) { */
	/* 	mOutputStream << pMsg; */
	/* 	return *this; */
	/* } */

	/* Log& Log::operator <<(long pVar) { */
	/* 	mOutputStream << pMsg; */
	/* 	return *this; */
	/* } */

	/* Log& Log::operator <<(int pVar) { */
	/* 	mOutputStream << pVar; */
	/* 	return *this; */
	/* } */

	/* Log& Log::operator <<(double pVar) { */
	/* 	mOutputStream << pVar; */
	/* 	return *this; */
	/* } */

	/* Log& Log::operator <<(const std::string& pMsg) { */
	/* 	mOutputStream << pMsg; */
	/* 	return *this; */
	/* } */
}

