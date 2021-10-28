/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {

  //check if table not equal NULL
  if (table != NULL) {
    //initilize every cell
    int i = 0;
    while (i < HT_SIZE) {
      (*table)[i] = NULL;
      i++;
    }
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {

  ht_item_t *hi = (*table)[get_hash(key)];
  //iterate through keys
  while (hi != NULL) {
    //compare keys
    if (!strcmp(hi->key, key)) {
      return hi;
    }
    hi = hi->next;
  }
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {

  ht_item_t *hi = ht_search(table, key);
  //check if cell with given key already existing
  if (hi != NULL) {
    hi->value = value;
  } else {
    ht_item_t *nhi = malloc(sizeof(ht_item_t));
    //check if allocation was successful
    if (nhi != NULL) {
      //assign value, key and next cell on the new cell
      nhi->value = value;
      nhi->key = key;  
      hi = (*table)[get_hash(key)];
      if (hi != NULL) {
        nhi->next = hi;
      } else {
        nhi->next = NULL;
      }
      //put new cell on the start of the list
      (*table)[get_hash(key)] = nhi;
    }
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {

  ht_item_t *hi = ht_search(table, key);
  //check if item has been founded
  if (hi != NULL) {
    return &(hi->value);
  } else {
    return NULL;
  }
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {

  ht_item_t *hi = (*table)[get_hash(key)], *phi = NULL;
  //cycle trough table and check if given key exist
  while (hi != NULL) {
    ht_item_t *nhi = hi->next;
    if (!strcmp(hi->key, key)) {
      if (phi != NULL) {
        phi->next = nhi;;
      } else {
        (*table)[get_hash(key)] = nhi;
      }
      //free memory of founded cell
      free(hi);
      //end cycle
      break;
    }
    phi = hi;
    hi = hi->next;
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  //iterate trough table
  int i = 0;
  while (i < HT_SIZE) {
    ht_item_t *hi = (*table)[i];
    while (hi != NULL) {
      ht_item_t *nhi = hi->next;
      //free alocated memory of the cell
      free(hi);
      hi = nhi;
    }
    (*table)[i] = NULL;
    i++;
  }
}
