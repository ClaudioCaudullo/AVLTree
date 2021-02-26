// C++ program of AVL tree that 
// handles duplicates 
#include <stdio.h> 
#include <stdlib.h> 

// Struttura di un nodo dell'albero 
struct node { 
	int key; 
	struct node* left; 
	struct node* right; 
	int height; 
	int count; 
}; 

// Metodo che restituisce l'altezza dell'albero
int height(struct node* N) 
{ 
	if (N == NULL) 
		return 0; 
	return N->height; 
} 

// ritorno l'intero più grande
// dai due interi in input
int max(int a, int b) 
{ 
	return (a > b) ? a : b; 
} 

/* Funzione di utilità che crea
   il nuovo nodo con la key data e mette   
   i puntatori destri e sinistri a NULL*/
struct node* newNode(int key) 
{ 
	struct node* node = (struct node*) 
		malloc(sizeof(struct node)); 
	node->key = key; 
	node->left = NULL; 
	node->right = NULL; 
	node->height = 1; // l'altezza di un nuovo nodo è 1
	node->count = 1; // prima apparizione dell chiave nell'albero
	return (node); 
} 

// Rotazione a destra del sottoalbero con radice y   
struct node* rightRotate(struct node* y) 
{ 
	struct node* x = y->left; 
	struct node* T2 = x->right; 

	// Rotazione a destra
	x->right = y; 
	y->left = T2; 

	// Aggiorno le altezze
	y->height = max(height(y->left), height(y->right)) + 1; 
	x->height = max(height(x->left), height(x->right)) + 1; 

	return x; 
} 

// Rotazione a sinistra del sottoalbero con radice x  
struct node* leftRotate(struct node* x) 
{ 
	struct node* y = x->right; 
	struct node* T2 = y->left; 

	// Ratazione a sinistra
	y->left = x; 
	x->right = T2; 
	// Aggiorno le altezze 
	x->height = max(height(x->left), height(x->right)) + 1; 
	y->height = max(height(y->left), height(y->right)) + 1; 

	return y; 
} 

// Prendo il fattore di bilancio del nodo n  
int getBalance(struct node* N) 
{ 
	if (N == NULL) 
		return 0; 
	return height(N->left) - height(N->right); 
} 

// Inserimento di una chiave nell'albero 
// con radice node 
struct node* insert(struct node* node, int key) 
{ 
	// BST
	if (node == NULL) 
		return (newNode(key)); 

	// Se già la chiave è presente, ne aumento il count
	if (key == node->key) { 
		(node->count)++; 
		return node; 
	} 

	// altrimenti la inserisco
	if (key < node->key) 
		node->left = insert(node->left, key); 
	else
		node->right = insert(node->right, key); 

	// AGGIORNO L'ALTEZZA DEL NODO APPENA INSERITO
	node->height = max(height(node->left), height(node->right)) + 1; 

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
	if (balance > 1 && key > node->left->key) { 
		node->left = leftRotate(node->left); 
		return rightRotate(node); 
	} 

	// Right Left Case 
	if (balance < -1 && key < node->right->key) { 
		node->right = rightRotate(node->right); 
		return leftRotate(node); 
	} 

	return node; 
} 

// Cerco il nodo con la key minima 
struct node* minValueNode(struct node* node) 
{ 
	struct node* current = node; 

    /* non cerco in tutto l'albero ma arrivo 
    fino all'estremo nodo a sinistra */
	while (current->left != NULL) 
		current = current->left; 

	return current; 
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
		if (root->count > 1) { 
			(root->count)--; 
			return root; 
		} 
		// Altrimenti elimino il nodo

        // nodo con al massimo un figlio
		if ((root->left == NULL) || (root->right == NULL)) { 
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
	if (root == NULL) 
		return root; 

  
    // Aggiorno l'altezza del nodo corrente
	root->height = max(height(root->left), height(root->right)) + 1; 

    /* Prendo il fattore di bilancio del nuovo nodo 
       per vedere se e dove quest'ultimo non è bilanciato */
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
		printf("%d(%d) ", root->key, root->count); 
		preOrder(root->left); 
		preOrder(root->right); 
	} 
} 

int main() 
{ 
	struct node* root = NULL; 

	root = insert(root, 9); 
	root = insert(root, 5); 
	root = insert(root, 10); 
	root = insert(root, 5); 
	root = insert(root, 9); 
	root = insert(root, 7); 
	root = insert(root, 17); 

	printf("Pre order: \n"); 
	preOrder(root); 

	root = deleteNode(root, 9); 

	printf("\nPre order dopo l'eliminazione del 9 \n"); 
	preOrder(root); 

	return 0; 
} 
