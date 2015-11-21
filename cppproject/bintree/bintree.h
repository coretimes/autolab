class BinaryTree
{
public:
  BinaryTree();
  bool insert(int nData);
  BinaryTree * find(int nData);
  void print(); 
private:
  void updatePrarentDepth();
   
private:
  BinaryTree *m_pLeftChild;
  BinaryTree *m_pRightChild;
  BinaryTree *m_pParent;
  int m_nDepth;
  int m_nRoot;
};
