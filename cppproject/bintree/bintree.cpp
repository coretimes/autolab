#include<assert.h>
#include<iostream>
#include<stddef.h>
#include "bintree.h"
BinaryTree::BinaryTree()
: m_pLeftChild(NULL)
, m_pRightChild(NULL)
, m_pParent(NULL)
, m_nRoot(-1)
, m_nDepth(0)
{
  
}

bool BinaryTree::insert(int nData)
{
  if(nData < 0 || nData == m_nRoot) 
  {
    return false;
  }
  if(m_nRoot == -1)
  {
    m_nRoot = nData;
    return false;
  }
  if (nData > m_nRoot)
  {
    if(m_pRightChild == 0)
    {
      m_pRightChild = new BinaryTree();
    }
      m_pRightChild->insert(nData);
  }

  if (nData < m_nRoot)
  {
    if(m_pLeftChild == 0)
    {
      m_pLeftChild=new BinaryTree();
    }
    m_pLeftChild->insert(nData);
  }
  return true;
}
void BinaryTree::print()
{
   if (m_nRoot < 0)
   {
     return;
   }
   if(m_pLeftChild != NULL)
   {
     m_pLeftChild->print();
   }
   std::cout << m_nRoot;
   std::cout << "," ;
   if(m_pRightChild != NULL)
   {
     m_pRightChild->print();
   }
}

void BinaryTree::updatePrarentDepth()
{

}
