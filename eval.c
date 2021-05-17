#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "eval.h"

liste_token * init_liste_token() {
  liste_token * tokList = malloc(sizeof(liste_token *));
  token * tok = malloc(sizeof(token *));

  if(tokList == NULL || tok  == NULL) exit(11);

  tok->type = -1;
  tok->valeur = 0;
  tok->next = NULL;
  tokList->premier = tok;

  return tokList;
}

void inserer(liste_token * tokList, int type, char valeur){

  token * new = malloc(sizeof(token *));

  if(tokList == NULL || new == NULL) exit(12);

  new->type = type;
  new->valeur = valeur;
  new->next = tokList->premier;
  tokList->premier = new;
}

void supprimer(liste_token * tokList){
  if(tokList->premier != NULL){
    token * trash = tokList->premier;
    tokList->premier = tokList->premier->next;
    free(trash);
  }
}

void free_token(token * tok){
  if(tok->next != NULL) free_token(tok->next);
  free(tok);
}

void free_liste_token(liste_token tokList){
  free_token(tokList.premier);
}

liste_token inverser_liste(liste_token tokList){
  liste_token * reverseList = init_liste_token();
  token * tmptok = tokList.premier;

  while(tmptok->next){
    inserer(reverseList,tmptok->type, tmptok->valeur);
    tmptok = tmptok->next;
  }

  free_liste_token(tokList);

  return *reverseList;
}

liste_token string_to_token (char *string){

  liste_token * tokList = init_liste_token();
  int i, j, k;
  for(i = 0; string[i] != '\0'; i++){

    if(string[i] == ' ' || string[i] == '\n') continue;        
    
    if(i+1 < MAX-1) j = i+1; else j = MAX-1;
    if(i+2 < MAX-1) k = i+2; else k = MAX-1;

    if(string[i] == '0')
      inserer(tokList, CONSTANTE, '0');
    else if(string[i] == '1')
      inserer(tokList, CONSTANTE, '1');
    else if(string[i] == '+')
      inserer(tokList, OP_BINAIRE, '+');
    else if(string[i] == '.')
      inserer(tokList, OP_BINAIRE, '.');
    else if(string[i] == '(')
      inserer(tokList, PARENTHESE, '(');
    else if(string[i] == ')')
      inserer(tokList, PARENTHESE, ')');
    else if(string[i] == '=' && string[j] == '>'){
      inserer(tokList, OP_BINAIRE, '>');
      i++;
    }
    else if(string[i] == '<' && string[j] == '=' && string[k] == '>'){
      inserer(tokList, OP_BINAIRE, '=');
      i+=2;
    }
    else if(string[i] == 'N' && string[j] == 'O' && string[k] == 'N'){
      inserer(tokList, OP_UNAIRE, 'N');
      i+=2;
    }
    else
      inserer(tokList, INCONNU, '?');
    
  }

  *tokList = inverser_liste(*tokList);
  
  return *tokList;
}

void afficher_liste_token(liste_token tokList){
  token * tok = tokList.premier;

  while(tok){
    printf("%c ", tok->valeur);
    tok = tok->next;
  }
}

int verif_parentheses(token * tok){
  int c = 0;
  token * tmptok = malloc(sizeof(token *));
  tmptok = tok;
  while(tmptok != NULL){
    if(tmptok->valeur == '(') c++;
    else if(tmptok->valeur == ')') c--;
    tmptok = tmptok->next;
  }
  
  if(c==0) return 1;
  else return 0;
}

int verif_cond(token* tok){
  token * tmptok = malloc(sizeof(token *));
  tmptok = tok;

  while(tmptok->next != NULL ){

    // Après une constante, il ne faut pas de constante, d'op unaire ou de parenthère ouvrante
    if (tmptok->type == CONSTANTE && (tmptok->next->type == CONSTANTE || tmptok->next->valeur == '(') ) return 0;

    // Après un opérateur binaire il ne faut pas de parenthèse fermante ou d'op binaire
    else if(tmptok->type == OP_BINAIRE && (tmptok->next->valeur == ')' || tmptok->next->type == OP_BINAIRE ) ) return 0;

    //  Après un opérateur unaire, il ne faut pas d'op unaire ou binaire ou de parenthèse fermante
    else if(tmptok->type == OP_UNAIRE && ( tmptok->next->type == OP_UNAIRE || tmptok->next->type == OP_BINAIRE || tmptok->next->valeur == ')' ) ) return 0;

    // Après une parenthèse ouvrante, il ne faut pas d'opérateur binaire
    else if(tmptok->valeur == '(' && tmptok->next->type == OP_BINAIRE ) return 0;

    // Après une parenthèse fermante, il ne faut pas d'op unaire ou de constante,
    else if(tmptok->valeur == ')' && (tmptok->next->type == OP_UNAIRE || tmptok->next->type == CONSTANTE ) ) return 0;

    //Aucun Symbole hors alphabet autorisé
    else if(tmptok->type == INCONNU) return 0;

    tmptok = tmptok->next;
  }
  return 1;
}

int verif_fin(liste_token* liste){
  *liste = inverser_liste(*liste);
  if(liste->premier->type == OP_BINAIRE || liste->premier->type == OP_UNAIRE || liste->premier->valeur == '('){
     *liste = inverser_liste(*liste);
     return 0;
  }
  else{
    *liste = inverser_liste(*liste);
    return 1;
  }
}

int verif_expression(liste_token* liste){
  return (verif_parentheses(liste->premier) && verif_cond(liste->premier) && verif_fin(liste));
}


liste_token infix_to_prefix(liste_token tokList){
  
  liste_token * prefix = init_liste_token();
  liste_token * stack = init_liste_token();

  inserer(&tokList, PARENTHESE, '(');
  tokList = inverser_liste(tokList);
  inserer(&tokList, PARENTHESE, ')');
  tokList = inverser_liste(tokList);
  token * tmptok = tokList.premier;

  
  while(tmptok){
    if(tmptok->valeur == '=' || tmptok->valeur == '>'){
      while(stack->premier->valeur == '.' || stack->premier->valeur == '+' || stack->premier->valeur == 'N'){
      inserer(prefix, stack->premier->type, stack->premier->valeur);
      supprimer(stack);
      }
      inserer(stack, tmptok->type, tmptok->valeur);
    }

    else if(tmptok->valeur == '+'){
      while(stack->premier->valeur == '.' || stack->premier->valeur == 'N'){
  inserer(prefix, stack->premier->type, stack->premier->valeur);
  supprimer(stack);
      }
      inserer(stack,tmptok->type,tmptok->valeur);
    }

    else if(tmptok->valeur == '.'){
      while(stack->premier->valeur == 'N'){
  inserer(prefix, stack->premier->type, stack->premier->valeur);
  supprimer(stack);
      }
      inserer(stack, tmptok->type, tmptok->valeur);
    }

    else if(tmptok->valeur == 'N')
      inserer(stack, tmptok->type, tmptok->valeur);
    
    else if(tmptok->type == CONSTANTE)
      inserer(prefix,tmptok->type,tmptok->valeur);
    
    else if(tmptok->type == PARENTHESE){
      if(tmptok->valeur == '(')
  inserer(stack, tmptok->type, tmptok->valeur);
      else {
  
  token * tmpstack = stack->premier;
  int lib = 1;
  while(tmpstack->valeur != '('){
    inserer(prefix, tmpstack->type, tmpstack->valeur);
    tmpstack = tmpstack->next;
    lib++;
  }
  
  while(lib){
    supprimer(stack);
    lib--;
  }
  
      }
    }
    tmptok = tmptok->next;
  }

  free_liste_token(*stack);
  free_liste_token(tokList);
  
  return *prefix;
}

arbre_token * prefix_to_tree(token ** tok){

  arbre_token * elt = malloc(sizeof(arbre_token *));
  elt->noeud = *tok;
  printf("%c ", elt->noeud->valeur);

  if(elt->noeud->type == CONSTANTE){
    return elt;
  }
  else{
    *tok = (*tok)->next;
    elt->fg = prefix_to_tree(tok);
    if(elt->noeud->valeur != 'N'){
      *tok = (*tok)->next;
      elt->fd = prefix_to_tree(tok);
    }
    return elt;
  }
}


int arbre_to_int(arbre_token abr){
  if(abr.noeud->type == CONSTANTE) return atoi(&(abr.noeud->valeur));
  else{
    if(abr.noeud->valeur == 'N') return !(arbre_to_int(*(abr).fg));

    else{
      int y = arbre_to_int(*(abr).fg);
      int x = arbre_to_int(*(abr).fd);
      if(abr.noeud->valeur == '+') return (x || y);
      else if(abr.noeud->valeur == '.') return (x && y);
      else if(abr.noeud->valeur == '=') return (x == y);
      else return !(!x && y);
    }
    
  }

}

int main(int argc, char ** argv) {
int resultat;
char *resbool = NULL;
  if(argc > 2){
    exit(EXIT_FAILURE);
  } 
  else{
    char *expression = malloc(sizeof(strlen(argv[1])));
    expression = argv[1];
   
    liste_token tokList = string_to_token(expression);
    printf("Liste de tokens : ");
    afficher_liste_token(tokList);
    printf("\n");

    int vf;
    vf=verif_expression(&tokList);
    printf("Vérificatton de l'expression: %d\n",vf); 

    if(vf == 1){
      tokList = infix_to_prefix(tokList);
      printf("Notation préfixe\n");
      afficher_liste_token(tokList);
      printf("\n");

      printf("Création de l'arbre\n");
      token * save = tokList.premier;
      arbre_token * arbre = prefix_to_tree(&tokList.premier);

      tokList.premier = save;

      if((resultat = arbre_to_int(*arbre))){
      resbool = "Vrai";
    
    }
      else{
      resbool = "Faux";
      }
    printf("\nRésultat du calcul: %s\n\n\n", resbool  );
    }

  else{
    printf("L'expression arithmétique n'est pas correcte\n\n\n");
  }
  
  free_liste_token(tokList);
  }
  return 0;
}





// FONCTIONS infix_to_prefix et prefix_to_tree AVANT LA QUESTION OPTIONELLE 

/*
liste_token infix_to_prefix(liste_token tokList){
  
  liste_token * prefix = init_liste_token();
  liste_token * stack = init_liste_token();

  inserer(&tokList, PARENTHESE, '(');
  tokList = inverser_liste(tokList);
  inserer(&tokList, PARENTHESE, ')');
  tokList = inverser_liste(tokList);
  token * tmptok = tokList.premier;

  
  while(tmptok){
    if(tmptok->type == OP_BINAIRE || tmptok->type == OP_UNAIRE)
      inserer(stack, tmptok->type, tmptok->valeur);
    else if(tmptok->type == CONSTANTE)
      inserer(prefix,tmptok->type,tmptok->valeur);
    else if(tmptok->type == PARENTHESE){
      if(tmptok->valeur == '(')
  inserer(stack, tmptok->type, tmptok->valeur);
      else {
  
  token * tmpstack = stack->premier;
  int lib = 1;
  while(tmpstack->valeur != '('){
    inserer(prefix, tmpstack->type, tmpstack->valeur);
    tmpstack = tmpstack->next;
    lib++;
  }
  
  while(lib){
    supprimer(stack);
    lib--;
  }
  
      }
    }
    tmptok = tmptok->next;
  }

  free_liste_token(*stack);
  free_liste_token(tokList);
  
  return *prefix;
}

 arbre_token * prefix_to_tree(token *tok){ 

   token * tmptok = tok; 
   arbre_token * elt = malloc(sizeof(arbre_token *)); 
   elt->noeud = tmptok; 
   printf("%c ", elt->noeud->valeur); 

   if(elt->noeud->type == CONSTANTE){ 
     return elt; 
   } 
   else{ 
     tmptok = tmptok->next; 
     elt->fg = prefix_to_tree(tmptok); 
     if(elt->noeud->valeur != 'N'){ 
       tmptok = tmptok->next; 
       elt->fd = prefix_to_tree(tmptok); 
     } 
     return elt; 
   } 
} 
*/