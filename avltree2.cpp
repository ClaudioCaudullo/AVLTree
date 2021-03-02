// AVL Tree con chiavi duplicate
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 

using namespace std;

// Struttura di un nodo dell'albero 
struct node { 
	int key; 
	int height; 
	int count; 
	struct node* left; 
	struct node* right; 
}; 

// Metodo che restituisce l'altezza dell'albero
int getHeight(struct node* n) 
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
struct node* newNode(int key) 
{ 
	struct node* n = (struct node*) malloc(sizeof(struct node)); 
	n->key = key; 
	n->height = 1; // l'altezza di un nuovo nodo è 1
	n->count = 1; // prima apparizione dell chiave nell'albero
	n->left = NULL; 
	n->right = NULL; 
	return (n); 
} 

// Rotazione a destra del sottoalbero con radice n  
struct node* rightRotate(struct node *n) 
{ 
	struct node *x = n->left; 
	struct node *temp = x->right; 

	// Rotazione a destra
	x->right = n; 
	n->left = temp; 

	// Aggiorno le altezze
	n->height = max(getHeight(n->left),getHeight(n->right)) + 1; 
	x->height = max(getHeight(x->left),getHeight(x->right)) + 1; 

	return x; 
} 

// Rotazione a sinistra del sottoalbero con radice n
struct node* leftRotate(struct node *n) 
{ 
	struct node *x = n->right; 
	struct node *temp = x->left; 

	// Ratazione a sinistra
	x->left = n; 
	n->right = temp; 

	// Aggiorno le altezze 
	n->height = max(getHeight(n->left),getHeight(n->right)) + 1; 
	x->height = max(getHeight(x->left),getHeight(x->right)) + 1; 

	return x; 
} 

// Prendo il fattore di bilancio del nodo n  
int getBalance(struct node* n) 
{ 
	if (n == NULL) return 0; 
	return getHeight(n->left) - getHeight(n->right); 
} 

// Inserimento di una chiave nell'albero 
// con radice node 
struct node* insert(struct node* n, int key) 
{ 
	// BST
	if (n == NULL) return (newNode(key)); 

	// Se già la chiave è presente, ne aumento il count
	if (key == n->key) 
	{ 
		(n->count)++; 
		return n; 
	} 

	// altrimenti la inserisco
	if (key < n->key) 
		n->left = insert(n->left, key); 
	else
		n->right = insert(n->right, key); 

	// Aggiorno l'altezza del nodo appena inserito
	n->height = max(getHeight(n->left), getHeight(n->right)) + 1; 

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
	if (balance > 1 && key > n->left->key) { 
		n->left = leftRotate(n->left); 
		return rightRotate(n); 
	} 

	// Right Left Case 
	if (balance < -1 && key < n->right->key) { 
		n->right = rightRotate(n->right); 
		return leftRotate(n); 
	} 

	return n; 
} 

// Cerco il nodo con la key minima 
struct node* minValueNode(struct node* n) 
{ 
	struct node* temp = n; 

    // non cerco in tutto l'albero ma arrivo 
    // fino all'estremo nodo a sinistra 
	while (temp->left != NULL) 
		temp = temp->left; 

	return temp; 
} 

// Classica delete di un nodo 
// a partire dalla radice root 
// e cercando il nodo con la chiave key  
struct node* deleteNode(struct node* root, int key) 
{ 
    // Classica BST delete

	if (root == NULL) 
		return root; 

    // Se la chiave è minore  della root
    // allora dovrò cercare dal sottoalbero sinistro  
	if (key < root->key) 
		root->left = deleteNode(root->left, key); 

    // Al contrario, se la chiave è maggiore della root 
    // allora dovrò cercare dal sottoalbero detro 
	else if (key > root->key) 
		root->right = deleteNode(root->right, key); 

    // Se infine la chiave è uguale a quella della root
    // dobbiamo cancellare proprio la root 
	else { 
		// Se il nodo è presente più di una volta,
		// decremento semplicemente il count.
		if (root->count > 1) 
		{ 
			(root->count)--; 
			return root; 
		} 
		// Altrimenti elimino il nodo

        // nodo con al massimo un figlio
		if ((root->left == NULL) || (root->right == NULL)) 
		{ 
			struct node* temp = root->left ? root->left : root->right; 
  
            // Nodo senza figli  
			if (temp == NULL) { 
				temp = root; 
				root = NULL; 
			} 
			else // Nodo con un figlio 
				*root = *temp;

			free(temp); 
		} 
		else { 
            // Nodo con due figli: cerco il successore
            // del nodo (il più piccolo nel sottoalbero destro)
			struct node* temp = minValueNode(root->right); 

            // Copio il valore del successore e lo elimino
			root->key = temp->key; 
			root->count = temp->count; 
			temp->count = 1; 

			root->right = deleteNode(root->right, temp->key); 
		} 
	} 

    // Se il nodo aveva solo la radice
    // ritorno null  
	if (root == NULL) return root; 

  
    // Aggiorno l'altezza del nodo corrente
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1; 

    // Prendo il fattore di bilancio del nuovo nodo 
    //   per vedere se e dove quest'ultimo non è bilanciato 
	int balance = getBalance(root); 

    // Come prima, se il nodo non è bilanciato 
    // ci sono 4 possibili casi  

	// Left Left Case 
	if (balance > 1 && getBalance(root->left) >= 0) 
		return rightRotate(root); 

	// Left Right Case 
	if (balance > 1 && getBalance(root->left) < 0) { 
		root->left = leftRotate(root->left); 
		return rightRotate(root); 
	} 

	// Right Right Case 
	if (balance < -1 && getBalance(root->right) <= 0) 
		return leftRotate(root); 

	// Right Left Case 
	if (balance < -1 && getBalance(root->right) > 0) { 
		root->right = rightRotate(root->right); 
		return leftRotate(root); 
	} 

	return root; 
} 

// Normale preOrder di un albero binario
// con l'aggiunta della visualizzazione 
// dell'altezza di ogni nodo 
void preOrder(struct node* root) 
{ 
	if (root != NULL) { 
		cout<<root->key<<"("<<root->count<<") "; 
		preOrder(root->left); 
		preOrder(root->right); 
	} 
} 

int main() 
{ 
	struct node* root = NULL; 

	root = insert(root, 12); 
	root = insert(root, 1); 
	root = insert(root, 14); 
	root = insert(root, 1); 
	root = insert(root, 12); 
	root = insert(root, 7); 
	root = insert(root, 20); 

	cout<<"Pre order: \n"; 
	preOrder(root); 

	root = deleteNode(root, 12); 

	cout<<"\nPre order dopo l'eliminazione del 12: \n"; 
	preOrder(root); 

	return 0; 
} 
