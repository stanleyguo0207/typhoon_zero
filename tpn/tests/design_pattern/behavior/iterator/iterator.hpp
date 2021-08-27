//
//           ┌┬┐┬ ┬┌─┐┬ ┬┌─┐┌─┐┌┐┌
//            │ └┬┘├─┘├─┤│ ││ ││││
//            ┴  ┴ ┴  ┴ ┴└─┘└─┘┘└┘
//
// This file is part of the typhoon Project.
// Copyright (C) 2021 stanley0207@163.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_ITERATOR_ITERATOR_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_ITERATOR_ITERATOR_H_

#include "../../../test_include.h"

namespace iterator {

template <typename>
class Iterator;

template <typename T>
class Arggregate {
 public:
  Arggregate() { std::cout << "Arggregate ctor..." << std::endl; }
  virtual ~Arggregate() { std::cout << "Arggregate dtor..." << std::endl; }

  virtual T &operator[](int index)      = 0;
  virtual void PushData(T data)         = 0;
  virtual Iterator<T> *CreateIterator() = 0;
  virtual int GetSize()                 = 0;
};

template <typename T>
class ConcreteArggregate : public Arggregate<T> {
 public:
  ConcreteArggregate() {
    std::cout << "ConcreteArggregate ctor..." << std::endl;
  }
  virtual ~ConcreteArggregate() {
    std::cout << "ConcreteArggregate dtor..." << std::endl;
  }

  // 越界外面处理
  T &operator[](int index) override {
    std::cout << "ConcreteArggregate operator[" << index << "]..." << std::endl;
    return this->data_[index];
  }

  void PushData(T data) override {
    std::cout << "ConcreteArggregate PushData " << data << " ..." << std::endl;
    this->data_.emplace_back(data);
  }

  Iterator<T> *CreateIterator() override;

  int GetSize() override {
    std::cout << "ConcreteArggregate GetSize..." << std::endl;
    return this->data_.size();
  }

 private:
  std::vector<T> data_;
};

template <typename T>
class Iterator {
 public:
  Iterator() { std::cout << "Iterator ctor..." << std::endl; }
  virtual ~Iterator() { std::cout << "Iterator dtor..." << std::endl; }

  virtual void First()  = 0;
  virtual void Next()   = 0;
  virtual T *CurrItem() = 0;
  virtual bool IsDone() = 0;
};

template <typename T>
class ConcreteIterator : public Iterator<T> {
 public:
  ConcreteIterator(Arggregate<T> *aggr) : aggr_(aggr) {
    std::cout << "ConcreteIterator ctor..." << std::endl;
  }
  virtual ~ConcreteIterator() {
    std::cout << "ConcreteIterator dtor..." << std::endl;
  }

  void First() override {
    std::cout << "ConcreteIterator dtor..." << std::endl;
    this->curr_ = 0;
  }

  void Next() override {
    std::cout << "ConcreteIterator dtor..." << std::endl;
    if (this->curr_ < this->aggr_->GetSize()) {
      ++this->curr_;
    }
  }

  T *CurrItem() override {
    std::cout << "ConcreteIterator dtor..." << std::endl;
    if (this->curr_ < this->aggr_->GetSize()) {
      return &(*aggr_)[this->curr_];
    } else {
      return nullptr;
    }
  }

  bool IsDone() override {
    std::cout << "ConcreteIterator dtor..." << std::endl;
    return this->curr_ >= this->aggr_->GetSize();
  }

 private:
  Arggregate<T> *aggr_{nullptr};
  int curr_{0};
};

//////////////////////////////////////////////////////////////////////////

template <typename T>
Iterator<T> *ConcreteArggregate<T>::CreateIterator() {
  std::cout << "ConcreteArggregate CreateIterator..." << std::endl;
  return new ConcreteIterator<T>(this);
}

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "iterator///////////////////////////////////////////////////////"
               "//////////////////"
            << std::endl;
  std::cout << std::endl;

  Arggregate<int> *aggr = new ConcreteArggregate<int>();
  aggr->PushData(1);
  aggr->PushData(2);
  aggr->PushData(3);

  Iterator<int> *iter = aggr->CreateIterator();

  for (iter->First(); !iter->IsDone(); iter->Next()) {
    std::cout << *iter->CurrItem() << std::endl;
  }

  delete iter;
  delete aggr;
}

}  // namespace iterator

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_ITERATOR_ITERATOR_H_
