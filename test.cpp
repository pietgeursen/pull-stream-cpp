#include <deque>
#include <vector>
#include <iterator>
#include <functional>
#include <iostream>
#include <cstdarg>
#include "gtest/gtest.h"

using namespace std;

template <typename T>
auto values(T & begin, T & end){
  return [&](bool abort, auto cb){

    if(begin != end){
      cb(false, *begin++);
    }
    else{
      cb(true, *begin);
    }
  };
};

template <typename T>
auto Map(auto & mapper){

  return [&](auto & read){
    return [&](bool abort, auto cb){
      read(abort, [&](bool end, T val){
        if(end)
          cb(true, val);
        else
          cb(false, mapper(val));
      });
    };    
  };

}

template <typename T>
auto log(auto read){

  std::function<void (bool, T)> more = [&](bool done, T val){
    if(!done){
      cout << val << endl; 
      read(false, more);
    } 
  };

  read(false, more);
}

template <typename T, typename... Targs>
auto pull(auto & source, T & sink, Targs... Fargs){
  return pull(sink, Fargs...)(source);
}

auto pull(auto & stream){
  return stream;
}

TEST(returnsSourceWhenNumArgsIsOne, pull) {

  vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  auto begin = vec.begin(); //these are on the stack and are destroyed when the function ends.
  auto end = vec.end();

  auto vals = values(begin, end);

  auto newVals = pull(vals);

  vals(false, [](bool done, auto val){
    ASSERT_EQ(val, 1);    
    ASSERT_FALSE(done);    
  });
}

TEST(combinesSourceWithThrough, pull) {

  vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  auto begin = vec.begin(); //these are on the stack and are destroyed when the function ends.
  auto end = vec.end();

  auto vals = values(begin, end);

  auto mapper = [&](int val){return val * 2;};
  auto timesTwo = Map<int>(mapper);

  auto newVals = pull(vals, timesTwo);

  newVals(false, [](bool done, auto val){
    ASSERT_EQ(val, 2);    
    ASSERT_FALSE(done);    
  });
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

TEST(CanMapValues, Map) {
  vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  auto begin = vec.begin(); //these are on the stack and are destroyed when the function ends.
  auto end = vec.end();
  auto vals = values(begin, end);
  auto mapper = [&](int val){return val * 2;};

  auto timesTwo = Map<int>(mapper);
  auto doubled = timesTwo(vals);

  doubled(false, [](bool done, auto val){
    ASSERT_EQ(val, 2);    
    ASSERT_FALSE(done);    
  });

  doubled(false, [](bool done, auto val){
    ASSERT_EQ(val, 4);    
    ASSERT_FALSE(done);    
  });

  doubled(false, [](bool done, auto val){
    ASSERT_TRUE(done);    
  });
}
