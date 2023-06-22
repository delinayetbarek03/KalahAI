//  Side.h
//  Project 3
//
//  Created by Delina Yetbarek on 5/22/23.

#ifndef SIDE_H
#define SIDE_H

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;


inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

//========================================================================
// Timer t;                 // create and start a timer
// t.start();               // restart the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};

//========================================================================

#include <iostream>
using namespace std;



// Advance jumpy timer only after jumpInterval calls to elapsed
class JumpyTimer
{
public:
  JumpyTimer(int jumpInterval)
   : m_jumpInterval(jumpInterval), m_callsMade(0)
  {
      actualElapsed();
  }
  double elapsed()
  {
      m_callsMade++;
      if (m_callsMade == m_jumpInterval)
      {
          m_lastElapsed = m_timer.elapsed();
          m_callsMade = 0;
      }
      return m_lastElapsed;
  }
  double actualElapsed()
  {
      m_lastElapsed = m_timer.elapsed();
      return m_lastElapsed;
  }
private:
  Timer m_timer;
  int m_jumpInterval;
  int m_callsMade;
  int m_lastElapsed;
};


//unsigned long f_helper(int n, double timeLimit, JumpyTimer& timer)
//{
//    unsigned long callCount = 1;
//    if (n == 0)
//        callCount += 0;  /// do a base case computation
//    else if (timeLimit <= 0)
//        callCount += 0;  //  do non-recursive heuristic computation
//    else
//    {
//        for (int branchesLeftToExplore = 10; branchesLeftToExplore > 0; branchesLeftToExplore--)
//        {
//              // Allocate remaining time limit equally among remaining braches to explore
//            double thisBranchTimeLimit = timeLimit / branchesLeftToExplore;
//
//              // Save branch start time so can compute the time the branch took
//            double startTime = timer.elapsed();
//              // Explore this branch
//            callCount += f_helper(n-1, thisBranchTimeLimit, timer);
//              // Adjust remaining time
//            timeLimit -= (timer.elapsed() - startTime);
//            if (timeLimit <= 0)
//                timeLimit = 0;
//        }
//    }
//    return callCount;
//}
//
//
//unsigned long f(int n)
//{
//    double timeLimit = 4990;  // 4.99 seconds; allow 10 ms for wrapping up
//    JumpyTimer timer(1000);
//    unsigned long result = f_helper(n, timeLimit, timer);
//    cout << timer.actualElapsed() << " ms" << endl;
//    return result;
//}



#endif /* SIDE_H */



