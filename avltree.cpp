//AVL tree 
#include<iostream> 
using namespace std; 
 
class Node 
{ 
	public: 
	    int key; 
	    Node *left; 
	    Node *right; 
	    int height; 
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
	Node* node = new Node(); 
	node->key = key; 
	node->left = NULL; 
	node->right = NULL; 
	node->height = 1; // l'altezza di un nuovo nodo è 1
	return(node); 
} 

// Rotazione a destra del sottoalbero con radice y  
Node *rightRotate(Node *y) 
{ 
	Node *x = y->left; 
	Node *T2 = x->right; 

	// Rotazione a destra
	x->right = y; 
	y->left = T2; 

	// Aggiorno le altezze
	y->height = max(getHeight(y->left),getHeight(y->right)) + 1; 
	x->height = max(getHeight(x->left),getHeight(x->right)) + 1; 

	return x; 
} 

// Rotazione a sinistra del sottoalbero con radice x 
Node *leftRotate(Node *x) 
{ 
	Node *y = x->right; 
	Node *T2 = y->left; 

	// Ratazione a sinistra
	y->left = x; 
	x->right = T2; 

	// Aggiorno le altezze 
	x->height = max(getHeight(x->left),getHeight(x->right)) + 1; 
	y->height = max(getHeight(y->left),getHeight(y->right)) + 1; 

	return y; 
} 

// Prendo il fattore di bilancio del nodo n 
int getBalance(Node *n) 
{ 
	if (n == NULL) return 0; 
	return getHeight(n->left) - getHeight(n->right); 
} 

// Inserimento di una chiave nell'albero 
// con radice node  
Node* insert(Node* node, int key) 
{ 
	// BST 
	if (node == NULL) return(newNode(key)); 

	if (key < node->key) node->left = insert(node->left, key); 
	else if (key > node->key) node->right = insert(node->right, key); 
	else return node; //non inserisco perchè non ci possono essere chiavi uguali

	// AGGIORNO L'ALTEZZA DEL NODO APPENA INSERITO
	node->height = 1 + max(getHeight(node->left),getHeight(node->right)); 

	/* Prendo il fattore di bilancio dell'antenato del nuovo nodo 
     per vedere dove quest'ultimo non è bilanciato*/
	int balance = getBalance(node); 

	// Se il nodo non è bilanciato , ci troviamo in uno di questi 4 casi

	// Left Left Case 
	if (balance > 1 && key < node->left->key) 
		return rightRotate(node); 

	// Right Right Case 
	if (balance < -1 && key > node->right->key) 
		return leftRotate(node); 

	// Left Right Case 
	if (balance > 1 && key > node->left->key) 
	{ 
		node->left = leftRotate(node->left); 
		return rightRotate(node); 
	} 

	// Right Left Case 
	if (balance < -1 && key < node->right->key) 
	{ 
		node->right = rightRotate(node->right); 
		return leftRotate(node); 
	} 

	return node; 
} 

// Cerco il nodo con la key minima 
Node * minValueNode(Node* node)  
{  
    Node* current = node;  
  
    /* non cerco in tutto l'albero ma arrivo 
    fino all'estremo nodo a sinistra */
    while (current->left != NULL)  
        current = current->left;  
  
    return current;  
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
    // allora dovrò cercare dal sottoalbero detro  
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
  

    root = insert(root, 9);  
    root = insert(root, 5);  
    root = insert(root, 10);  
    root = insert(root, 0);  
    root = insert(root, 6);  
    root = insert(root, 11);  
    root = insert(root, -1);  
    root = insert(root, 1);  
    root = insert(root, 2);  
  
    /* L'AVL Tree dovrebbe essere 
         9  
        / \  
        1 10  
       / \ \  
       0 5 11  
      / / \  
     -1 2 6  
    */
  
    cout << "Preorder : \n";  
    preOrder(root);  
  
    root = deleteNode(root, 10);  
  
    /* L'AVL Tree dopo la cancellazione del nodo 10  
         1  
        / \  
       0   9  
      /   / \  
     -1  5  11  
        / \  
        2 6  
    */
  
    cout << "\nPreorder dopo l'eliminazione di 10 : \n";  
    preOrder(root);  
  
    return 0;  
} 
