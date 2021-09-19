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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_LINKED_LIST_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_LINKED_LIST_H_

#include <iterator>

#include "define.h"

namespace tpn {

// 双向链表头
class LinkedListHead;

/// 双向链表元素
class LinkedListElement {
  friend class LinkedListHead;

 public:
  /// 构造函数
  LinkedListElement() = default;

  /// 析构函数
  ~LinkedListElement() { Delink(); }

  /// 是否有后置节点
  ///  @return 有后置节点返回true
  bool HasNext() const {
    return (next_ptr_ && nullptr != next_ptr_->next_ptr_);
  }

  /// 是否有前置节点
  ///  @return 有前置节点返回true
  bool HasPrev() const {
    return (prev_ptr_ && nullptr != prev_ptr_->prev_ptr_);
  }

  /// 是否在链上
  ///  @return 在链上返回true
  bool IsInList() const {
    return (nullptr != prev_ptr_ && nullptr != next_ptr_);
  }

  /// 获取后置节点
  ///  @return 后置节点存在返回后置节点，否则返回nullptr
  LinkedListElement *GetNext() { return HasNext() ? next_ptr_ : nullptr; }

  /// 获取后置节点
  ///  @return 后置节点存在返回后置节点，否则返回nullptr
  const LinkedListElement *GetNext() const {
    return HasNext() ? next_ptr_ : nullptr;
  }

  /// 获取前置节点
  ///  @return 前置节点存在返回前置节点，否则返回nullptr
  LinkedListElement *GetPrev() { return HasPrev() ? prev_ptr_ : nullptr; }

  /// 获取前置节点
  ///  @return 前置节点存在返回前置节点，否则返回nullptr
  const LinkedListElement *GetPrev() const {
    return HasPrev() ? prev_ptr_ : nullptr;
  }

  /// 获取后置节点不做判空检查
  ///  @return 返回后置节点
  LinkedListElement *GetNextNoCheck() { return next_ptr_; }

  /// 获取后置节点不做判空检查
  ///  @return 返回后置节点
  const LinkedListElement *GetNextNoCheck() const { return next_ptr_; }

  /// 获取前置节点不做判空检查
  ///  @return 返回前置节点
  LinkedListElement *GetPrevNoCheck() { return prev_ptr_; }

  /// 获取前置节点不做判空检查
  ///  @return 返回前置节点
  const LinkedListElement *GetPrevNoCheck() const { return prev_ptr_; }

  /// 当前节点前插入节点
  ///  @param[in]   elem_ptr    要插入元素节点
  void InsertBefore(LinkedListElement *elem_ptr) {
    elem_ptr->next_ptr_  = this;
    elem_ptr->prev_ptr_  = prev_ptr_;
    prev_ptr_->next_ptr_ = elem_ptr;
    prev_ptr_            = elem_ptr;
  }

  /// 当前节点后插入节点
  ///  @param[in]   elem_ptr    要插入元素节点
  void InsertAfter(LinkedListElement *elem_ptr) {
    elem_ptr->prev_ptr_  = this;
    elem_ptr->next_ptr_  = next_ptr_;
    next_ptr_->prev_ptr_ = elem_ptr;
    next_ptr_            = elem_ptr;
  }

  /// 链上断开
  void Delink() {
    if (!IsInList()) {
      return;
    }

    next_ptr_->prev_ptr_ = prev_ptr_;
    prev_ptr_->next_ptr_ = next_ptr_;
    next_ptr_            = nullptr;
    prev_ptr_            = nullptr;
  }

 private:
  LinkedListElement *next_ptr_{nullptr};  ///< 后置节点
  LinkedListElement *prev_ptr_{nullptr};  ///< 前置节点

  TPN_NO_COPYABLE(LinkedListElement)
  TPN_NO_MOVEABLE(LinkedListElement)
};

/// 双向链表头
class LinkedListHead {
  friend class LinkedListElement;

 public:
  /// 构造函数
  LinkedListHead() {
    first_.next_ptr_ = &last_;
    last_.prev_ptr_  = &first_;
  }

  /// 析构函数
  ~LinkedListHead() {}

  /// 判断链表是否为空
  ///  @return 链表为空返回true
  bool IsEmpty() const { return !first_.next_ptr_->IsInList(); }

  /// 获取链表头
  ///  @return 链表不为空返回链表头，否则返回nullptr
  LinkedListElement *GetFirst() {
    return (IsEmpty() ? nullptr : first_.next_ptr_);
  }

  /// 获取链表头
  ///  @return 链表不为空返回链表头，否则返回nullptr
  const LinkedListElement *GetFirst() const {
    return (IsEmpty() ? nullptr : first_.next_ptr_);
  }

  /// 获取链表尾
  ///  @return 链表不为空返回链表尾，否则返回nullptr
  LinkedListElement *GetLast() {
    return (IsEmpty() ? nullptr : last_.prev_ptr_);
  }

  /// 获取链表尾
  ///  @return 链表不为空返回链表尾，否则返回nullptr
  const LinkedListElement *GetLast() const {
    return (IsEmpty() ? nullptr : last_.prev_ptr_);
  }

  /// 链表头插入
  ///  @param[in]   elem_ptr    链表节点
  void InsertFirst(LinkedListElement *elem_ptr) {
    first_.InsertAfter(elem_ptr);
  }

  /// 链表尾插入
  ///  @param[in]   elem_ptr    链表节点
  void InsertLast(LinkedListElement *elem_ptr) { last_.InsertBefore(elem_ptr); }

  /// 获取链表长度
  uint32_t GetSize() const {
    if (!size_) {
      uint32_t result               = 0;
      const LinkedListElement *curr = GetFirst();
      while (curr) {
        ++result;
        curr = curr->GetNext();
      }
      return result;
    } else {
      return size_;
    }
  }

  /// 增加链表长度
  void IncSize() { ++size_; }

  /// 减少链表长度
  void DecSize() { --size_; }

  /// 双向链表迭代器
  template <class _Ty>
  class Iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = _Ty;
    using difference_type   = ptrdiff_t;
    using distance_type     = ptrdiff_t;
    using pointer           = _Ty *;
    using const_pointer     = const _Ty *;
    using reference         = _Ty &;
    using const_reference   = const _Ty &;

    Iterator() = default;
    Iterator(pointer ptr) : ptr_(ptr) {}

    Iterator &operator=(const_pointer const &other) {
      ptr_ = pointer(other);
      return *this;
    }

    reference operator*() { return *ptr_; }

    pointer operator->() { return ptr_; }

    Iterator &operator++() {
      ptr_ = ptr_->GetNext();
      return (*this);
    }

    Iterator &operator++(int) {
      iterator tmp = *this;
      ++*this;
      return (tmp);
    }

    Iterator &operator--() {
      ptr_ = ptr_->GetPrev();
      return (*this);
    }

    Iterator &operator--(int) {
      iterator tmp = *this;
      --*this;
      return (tmp);
    }

    bool operator==(const Iterator &other) const {
      return (ptr_ == other.ptr_);
    }

    bool operator!=(const Iterator &other) const { return (!(*this == other)); }

    bool operator==(const_pointer other) const { return (ptr_ == other); }

    bool operator!=(const_pointer other) const { return (!(*this == other)); }

    bool operator==(const_reference other) const { return (ptr_ == &other); }

    bool operator!=(const_reference other) const { return (ptr_ != &other); }

   protected:
    pointer ptr_{nullptr};  ///< 迭代器元素
  };

  using iterator = Iterator<LinkedListElement>;  ///< 迭代器

 private:
  LinkedListElement first_;  ///< 首节点
  LinkedListElement last_;   ///< 尾节点
  uint32_t size_{0};         ///< 链表中的节点数量

  TPN_NO_COPYABLE(LinkedListHead)
  TPN_NO_MOVEABLE(LinkedListHead)
};

/// 来自linux内核，去掉了检查的部分
/// cast a member of a structure out to the containing structure
///  @param[out]  ptr         this指针
///  @param[in]   Typename    类名
///  @param]in]   MemberName  类成员名
#define TPN_CONTAINER_OF(ptr, type, member) \
  ((type *)((uintptr_t)(ptr)-offsetof(type, member)))

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_LINKED_LIST_H_
