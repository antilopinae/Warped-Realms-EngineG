#pragma once

namespace EngineG::Network {

class Message
{
public:
  Message(): mTime() {}

  void SetTime(long int time) { mTime = time; }
  void SetTimeNow();

  long int GetTime() const { return mTime; }

  void SetData(int data) { mData = data; }
  int GetData() const { return mData; }
private:
  long int mTime;
  int mData;
};

}