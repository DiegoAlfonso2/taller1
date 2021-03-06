#ifndef LOCK_H_
#define LOCK_H_

#include "commonMutex.h"

class Lock {
private:
	Mutex *mutex;
public:
	explicit Lock(Mutex *mutex);
	~Lock();
};

#endif /* LOCK_H_ */
