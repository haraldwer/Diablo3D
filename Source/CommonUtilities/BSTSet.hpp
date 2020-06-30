#pragma once
#include <cassert>
#include <iostream>

namespace CommonUtilities
{
#pragma region Node
	template<class T>
	class BSTSetNode
	{
		template <class T>
		friend class BSTSet;
	public:
		BSTSetNode(const BSTSetNode&) = delete;
		BSTSetNode& operator=(const BSTSetNode&) = delete;
		const T& GetData() const;
		const BSTSetNode<T>* GetLeft() const;
		const BSTSetNode<T>* GetRight() const;
	private:
		BSTSetNode<T>();
		BSTSetNode<T>(const T& anElement);
		void Delete();
		BSTSetNode<T>* left;
		BSTSetNode<T>* right;
		T el;
	};

	template <class T>
	const T& BSTSetNode<T>::GetData() const
	{
		return el;
	}

	template <class T>
	const BSTSetNode<T>* BSTSetNode<T>::GetLeft() const
	{
		return left;
	}

	template <class T>
	const BSTSetNode<T>* BSTSetNode<T>::GetRight() const
	{
		return right;
	}

	template <class T>
	BSTSetNode<T>::BSTSetNode(): left(nullptr), right(nullptr)
	{
	}

	template <class T>
	BSTSetNode<T>::BSTSetNode(const T& anElement): left(nullptr), right(nullptr), el(anElement)
	{
	}

	template <class T>
	void BSTSetNode<T>::Delete()
	{
		if (left)
		{
			left->Delete();
			delete(left);
			left = nullptr;
		}
		if (right)
		{
			right->Delete();
			delete(right);
			right = nullptr;
		}
	}
#pragma endregion 
	
	template<class T>
	class BSTSet
	{
	public:
		BSTSet();
		~BSTSet();
		const BSTSetNode<T>* GetRoot() const;
		bool HasElement(const T& anElement) const;
		void Insert(const T& anElement);
		void Remove(const T& anElement);
		void DSWBalance();

	private:
		void DeleteByMerging(BSTSetNode<T>*& aNode);
		BSTSetNode<T>* Find(const T& anElement, const bool anAssertIfNotFound) const;
		void DeleteByCopying(BSTSetNode<T>*& aNode);
		
		void Compact(int anAmount);
		void RotateRight(BSTSetNode<T>* aGr, BSTSetNode<T>* aPar, BSTSetNode<T>* aCh);
		void RotateLeft(BSTSetNode<T>* aGr, BSTSetNode<T>* aPar, BSTSetNode<T>* aCh);
		
		BSTSetNode<T>* myRoot;
	};

	template <class T>
	BSTSet<T>::BSTSet(): myRoot(nullptr)
	{
	}

	template <class T>
	BSTSet<T>::~BSTSet()
	{
		if (myRoot)
		{
			myRoot->Delete();
			delete(myRoot);
			myRoot = nullptr;
		}
	}

	template <class T>
	const BSTSetNode<T>* BSTSet<T>::GetRoot() const
	{
		return myRoot;
	}

	template <class T>
	bool BSTSet<T>::HasElement(const T& anElement) const
	{
		return (Find(anElement, false) != nullptr);
	}

	template<class T>
	void BSTSet<T>::Insert(const T& anElement)
	{
		BSTSetNode<T>* p = myRoot, * prev = 0;
		while (p != 0)
		{  // find a place for inserting new node;
			prev = p;
			if (anElement < p->el)
				p = p->left;
			else if (prev->el < anElement)
				p = p->right;
			else return;
		}
		if (myRoot == 0)    // tree is empty;         
			myRoot = new BSTSetNode<T>(anElement);
		else if (anElement < prev->el)
			prev->left = new BSTSetNode<T>(anElement);
		else if (prev->el < anElement)
			prev->right = new BSTSetNode<T>(anElement);
	}

	template <class T>
	void BSTSet<T>::Remove(const T& anElement)
	{
		BSTSetNode<T>* node = myRoot, * prev = 0;
		while (node != 0)
		{
			if (node->el == anElement)
				break;
			prev = node;
			if (anElement < node->el)
				node = node->left;
			else node = node->right;
		}
		if (node != 0 && node->el == anElement)
			if (node == myRoot)
				DeleteByMerging(myRoot);
			else if (prev->left == node)
				DeleteByMerging(prev->left);
			else
				DeleteByMerging(prev->right);
		else if (myRoot != 0)
			assert(myRoot != nullptr && "element is not in the tree");
		else
			assert("tree is empty");
	}

	template<class T>
	void BSTSet<T>::DeleteByMerging(BSTSetNode<T>*& aNode)
	{
		BSTSetNode<T>* tmp = aNode;
		if (aNode != 0)
		{
			if (!aNode->right)            // node has no right child: its left             
				aNode = aNode->left;      // child (if any) is attached to its parent;
			else if (aNode->left == 0)    // node has no left child: its right             
				aNode = aNode->right;     // child is attached to its parent;
			else
			{                       // be ready for merging subtrees;
				tmp = aNode->left;      // 1. move left
				while (tmp->right != 0)// 2. and then right as far as possible;                
					tmp = tmp->right;
				tmp->right =           // 3. establish the link between                 
					aNode->right;        //    the rightmost node of the left subtree and the right subtree;
				tmp = aNode;            // 4.
				aNode = aNode->left;     // 5.
			}
			delete tmp;                 // 6.
		}
	}

	template <class T>
	BSTSetNode<T>* BSTSet<T>::Find(const T& anElement, const bool anAssertIfNotFound) const
	{
		BSTSetNode<T>* node = myRoot, * prev = 0;
		while (node != 0)
		{
			if (node->el == anElement)
				break;
			prev = node;
			if (anElement < node->el)
				node = node->left;
			else node = node->right;
		}
		if (node != 0 && node->el == anElement)
			if (node == myRoot)
				return (myRoot);
			else if (prev->left == node)
				return(prev->left);
			else
				return(prev->right);
		else if (myRoot != 0)
			assert(!anAssertIfNotFound || "element is not in the tree");
		else
			assert(!anAssertIfNotFound || "tree is empty");
		return nullptr;
	}

	template<class T>
	void BSTSet<T>::DeleteByCopying(BSTSetNode<T>*& aNode)
	{
		BSTSetNode<T>* previous, * tmp = aNode;
		if (aNode->right == 0)               // node has no right child;          
			aNode = aNode->left;
		else if (aNode->left == 0)           // node has no left child;          
			aNode = aNode->right;
		else
		{
			tmp = aNode->left;              // node has both children;
			previous = aNode;               // 1.
			while (tmp->right != 0)
			{      // 2.
				previous = tmp;
				tmp = tmp->right;
			}
			aNode->el = tmp->el;          // 3.
			if (previous == aNode)
				previous->left = tmp->left;
			else previous->right = tmp->left;  // 4.
		}
		delete tmp;                              // 5.
	}

	template <class T>
	void BSTSet<T>::DSWBalance()
	{
		if (!myRoot)
			return;
		
		BSTSetNode<T>* temp = myRoot;
		BSTSetNode<T>* last = nullptr;
		while (temp != nullptr)
		{
			if (temp->left != nullptr)
			{
				BSTSetNode<T>* newParent = temp->left;
				RotateRight(last, temp, temp->left);
				temp = newParent;
			}
			else
			{
				last = temp;
				temp = temp->right;
			}
		}

		int n = 0;
		auto node = myRoot;
		while (node)
		{
			n++;
			node = node->right;
		}

		int m = pow(2, static_cast<int>(floor(log2(n + 1)))) - 1;
		Compact(n - m);
		while (m > 1)
		{
			m = m / 2.0f;
			Compact(m);
		}
	}

	template <class T>
	void BSTSet<T>::Compact(int anAmount)
	{
		BSTSetNode<T>* gp = nullptr;
		BSTSetNode<T>* p = myRoot;
		BSTSetNode<T>* c = myRoot->right;
		for (; anAmount > 0; anAmount--)
		{
			if (c == nullptr)
				break;
			RotateLeft(gp, p, c);
			gp = c;
			p = gp->right;
			if (p == nullptr)
				break;
			c = p->right;
		}
	}

	template <class T>
	void BSTSet<T>::RotateRight(BSTSetNode<T>* aGr, BSTSetNode<T>* aPar, BSTSetNode<T>* aCh)
	{
		if (aGr != nullptr)
			aGr->right = aCh;
		else
			myRoot = aCh;
		aPar->left = aCh->right;
		aCh->right = aPar;
	}

	template <class T>
	void BSTSet<T>::RotateLeft(BSTSetNode<T>* aGr, BSTSetNode<T>* aPar, BSTSetNode<T>* aCh)
	{
		if (aGr != nullptr)
			aGr->right = aCh;
		else
			myRoot = aCh;
		aPar->right = aCh->left;
		aCh->left = aPar;
	}
	
	//rotateRight(Gr, Par, Ch)
	//	if Par is not the myRoot of the tree // i.e., if Gr is not null
	//	grandparent Gr of child Ch becomes Ch’s parent;
	//	right subtree of Ch becomes left subtree of Ch’s parent Par;
	//	node Ch acquires Par as its right child;

	//createBackbone(myRoot)
	//	tmp = myRoot;
	//	while (tmp != 0)      
	//		if tmp has a left child
	//			rotate this child about tmp; // hence the left child becomes parent of tmp;
	//			set tmp to the child that just became parent;
	//		else set tmp to its right child;

	//createPerfectTree()
	//	n = number of nodes;
	//	m = 2⎣lg(n + 1)⎦ - 1;
	//	make n - m rotations starting from the top of backbone;
	//	while (m > 1)      
	//		m = m / 2;
	//		make m rotations starting from the top of backbone;
}