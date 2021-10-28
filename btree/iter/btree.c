/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {

  (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

  //going through the tree to find needed key
  while (tree != NULL) {
    //if key was found, save his value
    if (tree->key == key) {
      *value = tree->value;
      return true;
    } else {
      //choose direction
      if (key > tree->key) {
        tree = tree->right;
      } else if (key < tree->key) {
        tree = tree->left;
      }
    }
  }
  //return false in case of failure
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  
  if (tree != NULL) {
    //going through tree
    //check if node is leaf
    while ((*tree) != NULL) {
      if (key == (*tree)->key) {
        (*tree)->value = value;
        break;
      //go in needed direction
      } else if (key > (*tree)->key) {
        tree = &((*tree)->right);
      } else if (key < (*tree)->key) {
        tree = &((*tree)->left);
      }
    }
    //if node with given key doesnt exist, create new leaf node
    if ((*tree) == NULL) {
      *tree = malloc(sizeof(struct bst_node));
      (*tree)->right = (*tree)->left = NULL;
      (*tree)->key = key;
      (*tree)->value = value;
    }
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  if (tree != NULL) {
    while ((*tree) != NULL) {
      //if rightmost node was founded
      if ((*tree)->right == NULL) {
        target->value = (*tree)->value;
        target->key = (*tree)->key;
        //if rightmost node have left children replace it with him
        //free alocated memory
        if ((*tree)->left == NULL) {
          free(*tree);
          *tree = NULL;
        } else {
          bst_node_t *leftNode = (*tree)->left;
          free(*tree);
          *tree = leftNode;
        }
        break;
      //going through right part of the tree
      } else {
        tree = &((*tree)->right);
      }
    }
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {

  while ((*tree) != NULL) {
    //if needed node was founded
    if ((*tree)->key == key) {
      //if node have only 1 children
      if ((*tree)->left == NULL && (*tree)->right != NULL) {
        bst_node_t *mNode = (*tree)->right;
        free(*tree);
        (*tree) = mNode;
      } else if ((*tree)->right == NULL && (*tree)->left != NULL) {
        bst_node_t *mNode = (*tree)->left;
        free(*tree);
        (*tree) = mNode;
      //if node is leaf
      } else if ((*tree)->right == NULL && (*tree)->left == NULL) {
        free(*tree);
        (*tree) = NULL;
      //if node have both childrens
      } else if ((*tree)->right != NULL && (*tree)->left != NULL) {
        bst_replace_by_rightmost((*tree), &((*tree)->left));
      }
    //going through tree to find needed node
    } else {
      if ((*tree)->key < key) {
        tree = &((*tree)->right);
      } else {
        tree = &((*tree)->left);
      }
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  //check if tree isnt empty
  if ((*tree) != NULL) {
    //create and initialize stack
    stack_bst_t deleting;
    stack_bst_init(&deleting);
    //deleteing each node of the tree until stack is empty
    while (!stack_bst_empty(&deleting) || (*tree) != NULL) {
      if ((*tree) == NULL) {
        *tree = stack_bst_pop(&deleting);
      }
      //save left node on the top of the stack
      if ((*tree)->left != NULL) {
        stack_bst_push(&deleting, (*tree)->left);
      }
      //delete node and free alocated memory
      bst_node_t *toDel = *tree;
      (*tree) = (*tree)->right;
      free(toDel);
    }
  }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  //push and print all left nodes of the tree
  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  //initialize stack for nodes
  stack_bst_t preord;
  stack_bst_init(&preord);
  bst_leftmost_preorder(tree, &preord);
  //going through tree with preorder traversal
  while (!stack_bst_empty(&preord)) {
    bst_node_t *act = stack_bst_pop(&preord);
    bst_leftmost_preorder(act->right, &preord);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  //push all left nodes of the tree
  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  //initialize tree
  stack_bst_t inord;
  stack_bst_init(&inord);
  bst_leftmost_inorder(tree, &inord);
  //going through tree with inorder traversal
  while (!stack_bst_empty(&inord)) {
    bst_node_t *act = stack_bst_pop(&inord);
    bst_print_node(act);
    bst_leftmost_inorder(act->right, &inord);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  //going through left nodes of the tree and save them
  //and true value for each of them on the stacks
  while (tree != NULL) {
    stack_bool_push(first_visit, true);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  //initialize stacks
  stack_bst_t postord;
  stack_bool_t postord_bool;
  stack_bst_init(&postord);
  stack_bool_init(&postord_bool);
  bst_leftmost_postorder(tree, &postord, &postord_bool);
  //going through tree with postorder traversal
  while (!stack_bst_empty(&postord)) {
    bst_node_t *act = stack_bst_pop(&postord);
    bool visit = stack_bool_pop(&postord_bool);
    //if node was already visited
    if(visit == false) {
      bst_print_node(act);
    } else {
      stack_bool_push(&postord_bool, false);
      stack_bst_push(&postord, act);
      bst_leftmost_postorder(act->right, &postord, &postord_bool);
    }
  }
}
