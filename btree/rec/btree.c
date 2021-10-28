/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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

  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  
  if (tree != NULL) {
    //in case of founded leaf
    if (tree->key == key) {
      *value = tree->value;
      return true;
    //going through tree
    } else {
      if (key > tree->key) {
        return bst_search(tree->right, key, value);
      } else {
        return bst_search(tree->left, key, value);
      }
    }
  //in case of failure in the search
  } else {
    return false;
  }
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

  if ((*tree) != NULL) {
    //check if leaf already existing
    if ((*tree)->key == key) {
      (*tree)->value = value;
    //going through the tree
    } else {
      if ((*tree)->key > key) {
        bst_insert(&((*tree)->left), key, value);
      } else {
        bst_insert(&((*tree)->right), key, value);
      }
    }
  //put leaf in the right position
  } else {
    *tree = malloc(sizeof(struct bst_node));
    (*tree)->right = (*tree)->left = NULL;
    (*tree)->key = key;
    (*tree)->value = value;
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  if ((*tree) != NULL) {
    //check if its a rightmost node
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
    //going through right part of the tree
    } else {
    bst_replace_by_rightmost(target, &((*tree)->right));
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {

  //check if tree isnt empty
  if ((*tree) != NULL) {
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
        bst_delete(&((*tree)->right), key);
      } else {
        bst_delete(&((*tree)->left), key);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  //going through tree
  if ((*tree) != NULL) {
    //delete all right nodes
    bst_dispose(&((*tree)->right));
    //delete all left nodes
    bst_dispose(&((*tree)->left));
    //free their memory
    free(*tree);
    (*tree) = NULL;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  //going through tree with preorder traversal
  if (tree != NULL) {
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  //going through tree with inorder traversal
  if(tree != NULL) {
    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  //going through tree with postorder traversal
  if (tree != NULL) {
    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
  }
}
