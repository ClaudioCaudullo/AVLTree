//AVL tree 
#include<iostream> 
using namespace std; 
 
class Node 
{ 
	public: 
	    int key; 
        int height; 
	    Node *left; 
	    Node *right; 
}; 

// Dato un nodo,ottengo la sua altezza
int getHeight(Node *n) 
{ 
	if (n == NULL) return 0; 
	return n->height; 
} 

// ritorno l'intero più grande
// dai due interi in input
int max(int a, int b) 
{ 
    if(a>b) return a;
    return b;
} 

/* Funzione di utilità che crea
   il nuovo nodo con la key data e mette   
   i puntatori destri e sinistri a NULL*/
Node* newNode(int key) 
{ 
	Node* n = new Node(); 
	n->key = key; 
    n->height = 1; // l'altezza di un nuovo nodo è 1
	n->left = NULL; 
	n->right = NULL; 
	return(n); 
} 

// Rotazione a destra del sottoalbero con radice n  
Node *rightRotate(Node *n) 
{ 
	Node *x = n->left; 
	Node *temp = x->right; 

	// Rotazione a destra
	x->right = n; 
	n->left = temp; 

	// Aggiorno le altezze
	n->height = max(getHeight(n->left),getHeight(n->right)) + 1; 
	x->height = max(getHeight(x->left),getHeight(x->right)) + 1; 

	return x; 
} 

// Rotazione a sinistra del sottoalbero con radice n
Node *leftRotate(Node *n) 
{ 
	Node *x = n->right; 
	Node *temp = x->left; 

	// Ratazione a sinistra
	x->left = n; 
	n->right = temp; 

	// Aggiorno le altezze 
	n->height = max(getHeight(n->left),getHeight(n->right)) + 1; 
	x->height = max(getHeight(x->left),getHeight(x->right)) + 1; 

	return x; 
} 

// Prendo il fattore di bilancio del nodo n 
int getBalance(Node *n) 
{ 
	if (n == NULL) return 0; 
	return getHeight(n->left) - getHeight(n->right); 
} 

// Inserimento di una chiave nell'albero 
// con radice n 
Node* insert(Node* n, int key) 
{ 
	// BST 
	if (n == NULL) return(newNode(key)); 

	if (key < n->key) n->left = insert(n->left, key); 
	else if (key > n->key) n->right = insert(n->right, key); 
	else return n; //non inserisco perchè non ci possono essere chiavi uguali

	// Aggiorno l'altezza del nodo appena inserito
	n->height = 1 + max(getHeight(n->left),getHeight(n->right)); 

	// Prendo il fattore di bilancio dell'antenato del nuovo nodo 
    // per vedere dove quest'ultimo non è bilanciato
	int balance = getBalance(n); 

	// Se il nodo non è bilanciato , ci troviamo in uno di questi 4 casi

	// Left Left Case 
	if (balance > 1 && key < n->left->key) 
		return rightRotate(n); 

	// Right Right Case 
	if (balance < -1 && key > n->right->key) 
		return leftRotate(n); 

	// Left Right Case 
	if (balance > 1 && key > n->left->key) 
	{ 
		n->left = leftRotate(n->left); 
		return rightRotate(n); 
	} 

	// Right Left Case 
	if (balance < -1 && key < n->right->key) 
	{ 
		n->right = rightRotate(n->right); 
		return leftRotate(n); 
	} 

	return n; 
} 

// Cerco il nodo con la key minima 
Node * minValueNode(Node* n)  
{  
    Node* temp = n;  
  
    // non cerco in tutto l'albero ma arrivo 
    // fino all'estremo nodo a sinistra 
    while (temp->left != NULL)  
        temp = temp->left;  
  
    return temp;  
}  
  
// Classica delete di un nodo 
// a partire dalla radice root 
// e cercando il nodo con la chiave key  
Node* deleteNode(Node* root, int key)  
{  
      
    // Classica BST delete 
    if (root == NULL) return root;  
  
    // Se la chiave è minore  della root
    // allora dovrò cercare dal sottoalbero sinistro   
    if ( key < root->key ) root->left = deleteNode(root->left, key);  
  
    // Al contrario, se la chiave è maggiore della root 
    // allora dovrò cercare dal sottoalbero destro  
    else if( key > root->key ) root->right = deleteNode(root->right, key);  
  
    // Se infine la chiave è uguale a quella della root
    // dobbiamo cancellare proprio la root 
    else
    {  
        // nodo con al massimo un figlio 
        if( (root->left == NULL) || (root->right == NULL) )  
        {  
            Node *temp;
            if(root->left!=NULL)  temp= root->left; 
            else if(root->right!=NULL)  temp= root->right; 
  
            // Nodo senza figli  
            if (temp == NULL)  
            {  
                temp = root;  
                root = NULL;  
            }  
            else // Nodo con un figlio  
            *root = *temp;
            free(temp);  
        }  
        else
        {  
            // Nodo con due figli: cerco il successore
            // del nodo (il più piccolo nel sottoalbero destro)
            Node* temp = minValueNode(root->right);  
  
            // Copio il valore del successore e lo elimino
            root->key = temp->key;  
            root->right = deleteNode(root->right,temp->key);  
        }  
    }  
  
    // Se il nodo aveva solo la radice
    // ritorno null  
    if (root == NULL) return root;  
  
    // Aggiorno l'altezza del nodo corrente  
    root->height = 1 + max(getHeight(root->left),getHeight(root->right));  
  
    /* Prendo il fattore di bilancio del nuovo nodo 
       per vedere se e dove quest'ultimo non è bilanciato */
    int balance = getBalance(root);  
  
    // Come prima, se il nodo non è bilanciato 
    // ci sono 4 possibili casi  
  
    // Left Left Case  
    if (balance > 1 && getBalance(root->left) >= 0)  
        return rightRotate(root);  
  
    // Left Right Case  
    if (balance > 1 && getBalance(root->left) < 0)  
    {  
        root->left = leftRotate(root->left);  
        return rightRotate(root);  
    }  
  
    // Right Right Case  
    if (balance < -1 && getBalance(root->right) <= 0)  
        return leftRotate(root);  
  
    // Right Left Case  
    if (balance < -1 && getBalance(root->right) > 0)  
    {  
        root->right = rightRotate(root->right);  
        return leftRotate(root);  
    }  
  
    return root;  
}  

// Normale preOrder di un albero binario
// con l'aggiunta della visualizzazione 
// dell'altezza di ogni nodo 
void preOrder(Node *root) 
{ 
	if(root != NULL) 
	{ 
		cout << root->key << " "; 
		preOrder(root->left); 
		preOrder(root->right); 
	} 
} 


int main()  
{  
Node *root = NULL;  
  

    root = insert(root, 7);  
    root = insert(root, 4);  
    root = insert(root, 8);  
    root = insert(root, 0);  
    root = insert(root, 6);  
    root = insert(root, 9);  
    root = insert(root, -1);  
    root = insert(root, 2);  
    root = insert(root, 3);  
  
    /* L'AVL Tree dovrebbe essere 
         7  
        / \  
        2 8  
       / \ \  
       0 4 9  
      / / \  
     -1 3 6  
    */
  
    cout << "Preorder : \n";  
    preOrder(root);  
  
    root = deleteNode(root, 8);  
  
    /* L'AVL Tree dopo la cancellazione del nodo 8  
         2  
        / \  
       0   7  
      /   / \  
     -1  4  9  
        / \  
        3 6  
    */
  
    cout << "\nPreorder dopo l'eliminazione di 8 : \n";  
    preOrder(root);  
  
    return 0;  
} 
