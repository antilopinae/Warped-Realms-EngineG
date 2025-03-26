#pragma once

#include "../Math/Math.hpp"
#include <cassert>
#include <string>
#include <stdio.h>

namespace EngineG::Test
{

  class TestVector {
    typedef Math::vec2 vec2;
    typedef Math::vec3 vec3;
  public:
    template<typename T, size_t Size>
    void testVector(){
      if constexpr(Size == 2) {
        T right = {1.0f, 0.0f};

        assert(right.x == 1.0f && "TestVector::right.x");
        assert(right.y == 0.0f && "TestVector::right.y");

        assert(right.asArray[0] == 1.0f && "TestVector::right.asArray[0]");
        assert(right.asArray[1] == 0.0f && "TestVector::right.asArray[1]");
      } else if constexpr(Size == 3){
        T right = {1.0f, 0.0f, 0.0f};

        assert(right.x == 1.0f && "TestVector::right.x");
        assert(right.y == 0.0f && "TestVector::right.y");
        assert(right.z == 0.0f && "TestVector::right.z");

        assert(right.asArray[0] == 1.0f && "TestVector::right.asArray[0]");
        assert(right.asArray[1] == 0.0f && "TestVector::right.asArray[1]");
        assert(right.asArray[2] == 0.0f && "TestVector::right.asArray[2]");
      }
    }

    TestVector(){
      try{
        testVector<vec2, 2>();
        testVector<vec3, 3>();
      }catch(...){
        assert(false && "TestVector::TestVector()");
      }
      printf("Vector passed!");
    }
  };

}
