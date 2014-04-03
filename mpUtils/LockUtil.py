#!/usr/bin/env python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>

import time
import multiprocessing
gChildRLock = multiprocessing.RLock()

def Locker(func):
    def __anon(*args, **kwargs):
        if gChildRLock.acquire(False): # If already held, don't block/wait until lock is released by current holder
                                       # Just go to the else-clause and the appropriate action will follow
            print('\033[47mAcquired lock', func, '\033[00m')
            results = dict()
            try:
                results['data'] = func(*args, **kwargs)
            except Exception as e:
                results['error'] = e
            else:
                # Release the lock
                print('\033[46mReleasedlock', func, '\033[00m')
                gChildRLock.release()
            finally:
                return results
        else:
            print('\033[41mCould not acquire lock. Try again\033[00m', func)
            return dict(error = Exception('Could not acquire lock. Try again later'), needsRetry=True)

    return __anon

def Retrieable(func, refreshTimeout=0.2):
    def __functor(*args, **kwargs):
        results = func(*args, **kwargs)
        if results and hasattr(results, 'get'):
            data = results.get('data', None)
            if data:
                print('Successful response from ', func, data)
                return data
            elif results.get('needsRetry', False):
                timeoutDelay = refreshTimeout if hasattr(refreshTimeout, '__divmod__') and refreshTimeout > 0.0 else 0.4
                print(func, '\033[33mRetrying after a refresh timeout of ', timeoutDelay, '\033[00m')
                time.sleep(timeoutDelay)
                return __functor(*args, **kwargs)
        else:
            return dict(
                msg='Could not decide whether to retry as \'get\' method undefined for data back', results=results
            )

    return __functor
