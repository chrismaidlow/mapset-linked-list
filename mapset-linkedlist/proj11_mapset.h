//Chris Maidlow - 4/23/2018 - Proj11Mapset

#ifndef MAP_SET
#define MAP_SET

#include<iostream>
using std::ostream; using std::cout; using std::endl;
#include<string>
using std::string;
#include<utility>
using std::pair;
#include<initializer_list>
using std::initializer_list;
#include<sstream>
using std::ostringstream;

//
// Node
//
template<typename K, typename V>
struct Node {
  K first;
  V second;
  Node *next = nullptr;
  
  Node() = default;
  Node(K,V);
  bool operator<(const Node&) const;
  bool operator==(const Node&) const;
  friend ostream& operator<<(ostream &out, const Node &n)
  {
    out << n.first << ":" << n.second;
    
    return out;
  }

};

//Node constructor 
template<typename K, typename V>
Node<K,V>::Node(K key, V value)
{
    first = key;
    second = value;
    Node *next = nullptr;

}

//Less-than operator
template<typename K, typename V>
bool Node<K,V>::operator<(const Node &n) const
{
    if(first < n.first)
    {
        return true;
    }
    return false;

}

//equal operator 
template<typename K, typename V>
bool Node<K,V>::operator==(const Node &n) const
{
    if(first == n.first)
    {
        return true;
    }

    return false; 

}

//
// MapSet
// 
template<typename K, typename V>
class MapSet{
 private:
  Node<K,V>* head_ = nullptr;
  Node<K,V>* tail_ = nullptr;  
  size_t sz_ = 0;
  Node<K,V>* find_key(K);

 public:
  MapSet()=default;
  MapSet(initializer_list< Node<K,V> >);
  MapSet (const MapSet&);
  MapSet operator=(MapSet);
  ~MapSet();
  size_t size();
  bool remove (K);  
  bool add(Node<K,V>);
  Node<K,V> get(K);
  bool update(K,V);  
  int compare(MapSet&);
  MapSet mapset_union (MapSet&);
  MapSet mapset_intersection(MapSet&);
  
  //Output operator for mapset
  friend ostream& operator<<(ostream &out, const MapSet &ms)
  {
    ostringstream oss;
    Node<K, V> *ptr;
    
    //cycle through list
    for(ptr = ms.head_; ptr != nullptr; ptr = ptr->next)

    //each entry sent to oss
	oss << (*ptr).first << ":" << (*ptr).second << ", ";
	
	//convert to string
    string str = oss.str();
    
    //remove last space and comma
    out << str.substr(0,str.size()-2);

	return out;
  }  
};

/*
    constructor - iterate through init list calling
    add for each pair 
*/
template<typename K, typename V>
MapSet<K,V>::MapSet(initializer_list< Node<K,V> > il)
{
    head_ = nullptr;
    tail_ = nullptr;
    sz_ = 0;
    
    //iterate through init calling add for each node
    for(auto i = il.begin(); i < il.end(); ++i)
    {
        Node<K, V> n(*i);
        auto result = add(n);
    }
}

//if mapset empty set mapset to empty
//otherwise step through list adding nodes
//produces copy
template<typename K, typename V>
MapSet<K,V>::MapSet(const MapSet &ms)
{
    //if empty
    if (ms.head_ == nullptr)
    {
	    head_ = nullptr;
	    tail_ = nullptr;
	    sz_ = 0;
    }
    
    //iterate through copying
    else
    {
	    head_ = new Node<K ,V>(ms.head_->first, ms.head_->second);
	    tail_ = head_;
	    Node<K,V>* sl_ptr = ms.head_->next;
	    Node<K,V>* new_node;
    	while (sl_ptr != nullptr)
    	{
	        new_node = new Node<K,V>(sl_ptr->first, sl_ptr->second);
	        tail_->next = new_node;
	        sl_ptr = sl_ptr->next;
	        tail_ = new_node;
	    }
    }
}

//init operator 
template<typename K, typename V>
MapSet<K,V> MapSet<K,V>::operator=(MapSet ms)
{
    swap(head_, ms.head_);
    swap(tail_, ms.tail_);
    return *this;

}	
/*
 walk down the list, moving head_ but remember it in to_del
 delete each node in turn, the set head_ and tail_
*/
template<typename K, typename V>
MapSet<K,V>::~MapSet()
{
    for (auto ptr=head_; ptr != nullptr;)
    {
        auto temp = ptr; // save ptr (pointer to current node) to delete later
        // update current node FIRST and then delete temp
        ptr = ptr->next;
        delete temp;
    }
}

//return size
template<typename K, typename V>
size_t MapSet<K,V>::size()
{
    return sz_;
}

/*
    if empty return null, iterate through list if equal return Node
    if key is greater than item and next node is either null or greater than key
    return node else continue. if other test fail check if it is greater than 
    nothing in the final loop
*/
template<typename K, typename V>
Node<K,V>* MapSet<K,V>::find_key(K key)
{
    //if no items present
    if(sz_ == 0)
    {
        return nullptr;
    }
  
    for(Node<K,V> *n = head_; n != nullptr; n = n->next)
    {
        // if key already present
        if(key == n->first)
        {
            return n;
        }
        // first element key is greater than - will trigger
        if (key > n->first)
        {
            //first checks if key is greater than all elements
            //if key is less than next element we found our node
            if (n->next == nullptr || key < n->next->first)
            {
                return n;
            }
            else
            {
                continue;
            }
        }
    }
    
    //if greater than none then return first item in list
    for(Node<K,V> *n = head_; n != nullptr; n = n->next)
    {
        if (key < n->first)
        { 
          return n;   
        }
    }
    
    return nullptr;
}
/*
    use find_key - if node not nullptr and key is equal
    key already present return false. if nothing is in 
    the list yet head_ and tail equal n increase sz 
    - if key less than find_key return value head_ = n 
    iterate size next = node_behind. Lastly, when inserting
    behind another entry n-> next becomes node_behind next
    then node behind next = node's address, iterate size
*/
template<typename K, typename V>
bool MapSet<K,V>::add(Node<K,V> m)
{
    auto node_behind = find_key(m.first);
    
    //if key already exists
    if (node_behind != nullptr && (*node_behind).first == m.first)
    {
        return false;
    }
    
    //allocate memory
    Node<K,V>* n = new Node<K, V>(m);
    
    //if nothing entered yet
    if(node_behind == nullptr)
    {
        n->next = nullptr;
        head_ = n;
        tail_ = n;
        ++sz_;
        return true;
    }
    
    //when added to the front
    if(node_behind->first > n->first)
    {
        head_ = n;
        n->next = node_behind;
        ++sz_;
        return true;
    }
    
    //when added behind another entry
    n->next = node_behind->next;
    
    (*node_behind).next = n;
    ++sz_;
   
    return true;
    
}

/*
    check if value in list if true remove it
*/
template<typename K, typename V>
bool MapSet<K,V>::remove(K key)
{
    for(Node<K, V> *n = head_; n != nullptr; n = n->next)
    {
        if (n->first == key)
        {
            //set node first and second to next first and second
            (*n).first = ((*(n->next)).first);
            (*n).second = ((*(n->next)).second);
            
            //set next to the address that next was originally pointing
            (*n).next = &(*(n->next)->next);
            
            return true;
        }
    }
    
    return false;
 
}

/*
    uses comparison and iteration to check if key in
    list
*/
template<typename K, typename V>
Node<K,V> MapSet<K,V>::get(K key)
{
    
    auto pntr = find_key(key);
    //check if key is in list
    if((*pntr).first == key)
    {
        //return value if there
        auto new_node = *pntr;
        return (new_node);
    }
    else
    {
        //return blank if not
        Node<K,V> blank = {"",0};
        return blank;
    }

}

/*
    check if key is present in list if true update value
*/
template<typename K, typename V>
bool MapSet<K,V>::update(K key, V value)
{
    auto pntr = find_key(key);
    
    //check if key is present
    if((*pntr).first == key)
    {
        //update second value
        (*pntr).second = value;
        return true;
    }
    else     
    {
        return false;
    }
}

/*
    iterate through each list making comparisons.
    if equal continue else return accordingly.
    Lastly, compare vector sizes if all else fails
*/

template<typename K, typename V>
int MapSet<K,V>::compare(MapSet &ms)
{
    Node<K,V> *n = head_;
    Node<K,V> *m = ms.head_;
    //cycle through both lists at the same time
    for(; n != nullptr && m != nullptr; n = n->next, m = m->next)
    {
        //if equal continue
        if(n->first == m->first)
        {
            continue;
        }
        //if calling is greater return 1
        else if(m->first < n->first)
        {
            return 1;
        }
        //if arg is greater return -1
        else if(m->first > n->first)
        {
            return -1;
        }
       
    }

    size_t size_arg = ms.size();
    size_t size_call = size();
    
    size_t standard;
    
    //if all else fails compare sizes 
    if(size() > ms.size())
    {
        return 1;
    }
    else if (size() < ms.size())
    {
        return -1;
    }
    else if (size() == ms.size() )
    {
        return 0;
    }
    
    return 0;
}


/*
    iterate through both lists adding nodes to new 
    list - add functions takes care of duplicates
*/
template<typename K, typename V>
MapSet<K,V> MapSet<K,V>::mapset_union(MapSet<K,V> &ms)
{
    MapSet<K,V> new_map;
    
    //iterate through first list and add
    for(Node<K, V> *n = head_; n != nullptr; n = n->next)
    {
        new_map.add(*n);
    }
    
    //iterate through second list and add
    for(Node<K, V> *n = ms.head_; n != nullptr; n = n->next)
    {
        new_map.add(*n);
    }
    
    return new_map;
    
        
}


/*
    iterate through calling list - see if pair is in
    argument list - if so, add to new list
*/
template<typename K, typename V>
MapSet<K,V> MapSet<K,V>::mapset_intersection(MapSet<K,V> &ms)
{
    
    MapSet<K,V> new_map;  
    
    //iterate through first list and if key is present
    //in other list then add to new list
    for(Node<K, V> *n = head_; n != nullptr; n = n->next)
    {
        auto key = ms.find_key((*n).first);
        
        if((*key).first == (*n).first)
        {
            new_map.add(*n);
        }
        
    }
    
    return new_map;
}

#endif
  