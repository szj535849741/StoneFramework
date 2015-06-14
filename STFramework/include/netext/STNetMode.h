#ifndef ST_NETMODE_H
#define ST_NETMODE_H

#include "base/STCommonDefine.h"

enum {
	DECFD, ADDFD, DESTROY, EXIT
};

class STNetMode {
public:
	virtual int32_t init(const STString&) = 0;
	virtual int32_t run() = 0;
	virtual int32_t destroy() = 0;
	virtual ~STNetMode() {
	}
public:
};



#endif