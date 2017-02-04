package CheckPoint;

import java.util.ArrayList;
import java.util.List;

import CheckPoint.MinHeap.Stack.Node;

public class MinHeap {
	
	private MinStack heap = new MinStack();
	
	public MinHeap(){}
	
	public void insert(int value){
		heap.push(value);
	}
	
	public Node min(){
		return heap.min();
	}

	public class Stack {
		
		class Node{
			public int value;	
			public Node next;

			public Node(int value){
				this.value = value;
			}
		}

		public Node head=null;

		public void push(int value){
			Node newObj = new Node(value);
			if (head==null){
				head=newObj;
			}
			else{
				newObj.next=head;
				head=newObj;
			}
		}

		public Node pop(){
			Node top = head;
			if (top!=null){
				head=head.next;	
				return top;
			}
			return null;
		}
		
		public Node peak(){
			if (head==null){return null;}
			else{
				return new Node(head.value);
			}
			
		}
	}
	
	public class MinStack extends Stack{
		
		Stack minStack = new Stack();
		
		public void push(int value){
			if (minStack.head==null){
				minStack.push(value);
			}
			else{
				if (minStack.peak().value>value){
					minStack.push(value);
				}
			}
			super.push(value);
		}
		
		public Node pop(){
			Node node = super.pop();
			if (minStack.peak().value==node.value){
				minStack.pop();
			}
			return node;
		}
		
		public Node peak(){
			return super.peak();
		}
		
		public Node min(){
			Node node=minStack.pop();
			if (node!=null){
				if (super.peak().value==node.value){
					super.pop();
				}
			}
			return node;
		}
	}

	public static void main(String[] args) {
		MinHeap mh = new MinHeap();
		mh.insert(6);	
		mh.insert(7);	
		mh.insert(4);
		mh.insert(5);
		mh.insert(3);
		Node current;
		while ((current=mh.min())!=null){
			System.out.println(current.value);
		}

	}


}
