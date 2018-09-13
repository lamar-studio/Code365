/*
 * Copyright (c) 2015 Josh Williams <theprime@codingprime.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the COPYING file for details.
 */
#include <sstream>

namespace VOsd {

	enum LogType {
		LogTypeError = 1,
		LogTypeNormal
	};
	enum LogLevel {
		ERROR = 1,
		INFO,
		DEBUG
	};

	class Log {
	public:
		Log();
		~Log(void);
		Log& Debug(void);
		Log& Error(void);

		template<typename T>
			Log& operator <<(T pVal) {
				mOutputStream << pVal;
				return *this;
			}
		/* Log& operator <<(const char* pMsg); */
		/* Log& operator <<(int pVar); */
		/* Log& operator <<(long pVar); */
		/* Log& operator <<(double pVar); */
		/* Log& operator <<(const std::string& pMsg); */
	private:

		LogType             mType;
		std::ostringstream  mOutputStream;
	};

#define ERROR VOsd::Log().Error()
#define DEBUG VOsd::Log().Debug()

} // End Namespace

