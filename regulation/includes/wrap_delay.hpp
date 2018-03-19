#pragma once

/*
#include <chrono>
#include <thread>


void delay(unsigned long ms)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}*/

/*Quelle des folgenden Codes: 
http://www.cplusplus.com/forum/unices/10491/ */


#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

  #include <windows.h>

  inline void delay( unsigned long ms )
    {
    Sleep( ms );
    }

#else  /* presume POSIX */

  #include <unistd.h>

  inline void delay( unsigned long ms )
    {
    usleep( ms * 1000 );
    }

#endif 

