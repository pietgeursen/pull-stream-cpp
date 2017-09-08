#include <deque>
#include <vector>
#include <iterator>
#include <functional>
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

template <typename T>
auto sink(auto read){

  std::function<void (bool, T)> more = [&](bool done, T val){
    if(!done){
      cout << val << endl; 
      read(false, more);
    } 
  };

  read(false, more);
}

TEST(CanCallValues, values) {
  vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  auto begin = vec.begin(); //these are on the stack and are destroyed when the function ends.
  auto end = vec.end();

  ASSERT_EQ(vec.size(), 2);    

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

TEST(SinkPrints, sink){
  vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  auto begin = vec.begin(); //these are on the stack and are destroyed when the function ends.
  auto end = vec.end();
  auto sauce = values(begin, end);

  sink<int>(sauce);
}
