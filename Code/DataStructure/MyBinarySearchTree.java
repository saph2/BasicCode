package CheckPoint;

import CheckPoint.BinarySearchTree.Node;

public class MyBinarySearchTree {
	
	class Node{
		
		public int key; 
		public Node left;
		public Node right;
		public Node parent;
		
		public Node(int key){
			this.key=key;
		}
	}
	
	
	public Node root;
	
	public MyBinarySearchTree(){ }
	
	public void insert (int key){
		if (find(key)!=null){return;}
		Node newNode = new Node(key);
		if (this.root==null){
			this.root=newNode;
		}
		else{
			Node current = this.root;
			Node parent = current;
			while (current!=null){
				parent=current;
				if (key<current.key){
					current=current.left;
				}
				else if (key>current.key){
					current=current.right;
				}
			}
			if (key<parent.key){
				parent.left=newNode;
				newNode.parent=parent;
			}
			else{
				parent.right=newNode;
				newNode.parent=parent;
			}
		}
		
	}
	
	public Node find(int key){
		if (this.root==null){return null;}
		if (this.root.key==key){
			return this.root;
		}
		else{
			Node current = this.root;
			while (current!=null){
				if (current.key==key){
					return current;
				}
				else if (current.key<key){
					current=current.right;
				}
				else{
					current=current.left;
				}
			}
		}
		return null;
	}
	
	public Node findPred(Node node){
		if (node.left!=null){
			Node current = node.left;
			while (current.right!=null){
				current=current.right;
			}
			return current;
		}
		else{
			if (node.parent.right==node){
				return node.parent;
			}
			else{
				Node current=node.parent;
				Node parent=current.parent;
				while (parent.right!=current){//up until first right
					current=current.parent;
					parent=current.parent;
				}
				return parent;
			}
		}
		
	}
	
	public Node findSucc(Node node){
		if (node.right!=null){
			Node current = node.right;
			while (current.left!=null){
				current=current.left;
			}
			return current;
		}
		else{
			if (node.parent.left==node){
				return node.parent;
			}
			else{
				Node current=node.parent;
				Node parent=current.parent;
				while (parent.left!=current){//up until first left
					current=current.parent;
					parent=current.parent;
				}
				return parent;
			}
		}
	}
	
	
	public void delete(int key){
		Node delNode = find(key);
		if (delNode==null){return;}
		else{
			if (delNode.right!=null && delNode.left!=null){//switch delNode with successor
				Node succ=findSucc(delNode);
				if (succ.parent.left==succ){
					succ.parent.left=null;
				}
				else{
					succ.parent.right=null;
				}
				delNode.key=succ.key;
			}
			else if (delNode.right!=null){//switch delNode with right child
				if (delNode.parent.left==delNode){
					delNode.parent.left=delNode.right;
				}
				else{
					delNode.parent.right=delNode.right;
				}
				delNode.right.parent=delNode.parent;
			}
			else if (delNode.left!=null){//switch delNode with left child
				if (delNode.parent.left==delNode){
					delNode.parent.left=delNode.left;
				}
				else{
					delNode.parent.right=delNode.left;
				}
				delNode.left.parent=delNode.parent;
			}
			else{//no children 
				if (delNode.parent.left==delNode){
					delNode.parent.left=null;
				}
				else{
					delNode.parent.right=null;
				}
			}
		}
	}
	
	public void display(Node root){
		if(root!=null){
			display(root.left);
			System.out.print(" " + root.key);
			display(root.right);
		}
	}
	
	public void printTree(){
		display(this.root);
	}
	

//	public static void main(String[] args) {
//		
//		MyBinarySearchTree t = new MyBinarySearchTree();
//		t.insert(20);
//		t.insert(10);
//		t.insert(30);
//		t.insert(5);
//		t.insert(17);
//		t.insert(25);
//		t.insert(40);
//		t.delete(10);
//		t.delete(17);
//		t.delete(5);
//		t.printTree();
//		
//	}

}
