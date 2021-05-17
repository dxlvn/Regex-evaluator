#define MAX 1024

typedef enum {
	      CONSTANTE,
	      OP_BINAIRE,
	      OP_UNAIRE,
	      PARENTHESE,
	      INCONNU
	      
} enum_type;

typedef struct token {

  int type;
  char valeur;
  struct token * next;

} token;


typedef struct liste_token {

  token * premier;

} liste_token;


typedef struct  arbre_token {

  token * noeud;
  struct arbre_token *fg, *fd;

} arbre_token;
