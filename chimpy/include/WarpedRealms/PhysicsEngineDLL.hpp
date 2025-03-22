#pragma once

#include "Export.hpp"
#include <string>
#include <vector>
#include <cstring>

namespace PhysicsEngine {
// Start Physic Server
extern "C" EXPORT void StartPhysics();
// Get position from one object and returns count of ticks
extern "C" EXPORT int GetObjectPosition(double* out_x, double* out_y, double* out_z);
// Stop Physic Server
extern "C" EXPORT void StopPhysics();

// callbacks
extern "C" EXPORT void SetCallback(void (*ptr)(void));
extern "C" EXPORT void CallMeMaybe(void);

// strings
extern "C" EXPORT bool IsLengthGreaterThan5(const char* value);
extern "C" EXPORT const char* GetName();

// structs

struct Shoe{
  int id;
  double size;
  const char* color;
  const char* brand;

  void buy() {
      size = 15.0;
  }
};

extern "C" EXPORT void BuyShoe(Shoe* shoe){
    if (!shoe) return;
    shoe->buy();
}


// classes
class WishList {
public:
    WishList(std::string name) : name_(name), items_() {}

    std::string getName();
    void setName(std::string name);
    int countItems();
    void addItem(std::string item);
    void removeItem(std::string item);

    void print();

private:
    std::string name_;
    std::vector<std::string> items_;
};


std::string WishList::getName() {
    return name_;
}

void WishList::setName(std::string name) {
    name_ = name;
}

void WishList::addItem(std::string item) {
    items_.push_back(item);
}

void WishList::removeItem(std::string item) {
    for (int i=0; i <items_.size(); ++i) {
        if (items_[i] == item) {
            items_.erase(items_.begin()+i);
            break;
        }
    }
}

int WishList::countItems() {
    return items_.size();
}

void WishList::print() {
    // some code here
}


// WishListWrapper

extern "C" EXPORT WishList* CreateWishList(const char* name) {
    return new WishList(name);
}

extern "C" EXPORT void DeleteWishList(WishList* wishList) {
    delete wishList;
}

extern "C" EXPORT const char* GetWishListName(WishList* wishList) {
    static std::string name;
    name = wishList->getName();
    return name.c_str();
}

extern "C" EXPORT void SetWishListName(WishList* wishList, const char* name) {
    wishList->setName(name);
}

extern "C" EXPORT void StartServer();

extern "C" EXPORT void StartGameEngine();





}
