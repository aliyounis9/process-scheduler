//	This is an updated version of code originally
//  created by Frank M. Carrano and Timothy M. Henry.
//  Copyright (c) 2017 Pearson Education, Hoboken, New Jersey.

/** @file QueueADT.h */
#ifndef QUEUE_ADT_
#define QUEUE_ADT_

template<class ItemType>
class QueueADT
{
	int Count ; 

public:
   virtual bool isEmpty() const = 0;
   virtual bool enqueue(const ItemType& newEntry) = 0;
   virtual bool dequeue(ItemType& FrontEntry) = 0;
   virtual bool peek(ItemType& FrontEntry) const = 0;
   void setCount(int x) {
	   Count =x ; 
   }
   int getCount () const{return Count;}
   virtual ~QueueADT() { }
}; 

#endif