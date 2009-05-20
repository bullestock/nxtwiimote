/*
 * core.h
 *
 *  Created on: Feb 6, 2009
 *      Author: csp
 */

#ifndef CORE_H_
#define CORE_H_
#include <sys/time.h>

namespace nxt
{

  // linux only maybe we should switch to boost timer class later, or let the preprocessor do the work
  // in milliseconds
  class Timer {
  public:
    Timer() {
      timeval tim;
      gettimeofday(&tim, NULL);
      sTime =tim.tv_sec+(tim.tv_usec/1000000.0);
    }

    ~Timer() { }

    int elapsed() const {
      timeval tim;
      gettimeofday(&tim, NULL);
      double cTime =tim.tv_sec+(tim.tv_usec/1000000.0);
      return (int)((cTime - sTime) *1000);
    }
    private:
      double sTime;
  };


  static void _sleep(unsigned int  useconds )
  {
    // 1 milliseconds = 1000 microsecond.
        // Windows Sleep uses miliseconds
        // linux usleep uses microsecond
        // and since the default
        // usage of the code was under windows
        // so the argument is   coming in millisecond.
        usleep( useconds * 1000 );
  }
}
#endif /* CORE_H_ */
