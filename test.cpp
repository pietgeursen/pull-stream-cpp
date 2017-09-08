#include <deque>
#include <vector>
#include <iterator>
#include <iostream>
#include "gtest/gtest.h"

using namespace std;

template <typename T>
auto values(T & begin, T & end){
  return [&](bool abort, auto cb){

    if(begin != end){
      cb(false, *begin++);
    }
    else{
      cb(true, NULL);
    }
  };
};

TEST(CanCallValues, values) {
  vector<int> deq;
  deq.push_back(1);
  deq.push_back(2);
  auto begin = deq.begin(); //these are on the stack and are destroyed when the function ends.
  auto end = deq.end();

  ASSERT_EQ(deq.size(), 2);    

  auto vals = values(begin, end);

  vals(false, [](bool done, auto val){
    ASSERT_EQ(val, 1);    
    ASSERT_FALSE(done);    
  });

  vals(false, [](bool done, auto val){
    ASSERT_EQ(val, 2);    
    ASSERT_FALSE(done);    
  });

  vals(false, [](bool done, auto val){
    ASSERT_TRUE(done);    
  });
}
