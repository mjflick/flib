
struct PersonObj
{
  char name[40]; 
  char *(*setname)(struct PersonObj *, const char *);
  char *(*getname)(struct PersonObj *);
  void (*print)(struct PersonObj *);
};
typedef struct PersonObj Person;

/* below is evil */  

char *setname(Person *P, const char *name)
{
  if(name)
    strcpy(P->name, name);

  return P->name;
}

char *getname(Person *P)
{
  return P->name;
}

void fprint (Person *P)
{
  printf("Wisdom motivation: '%s'\n", (char *) P->name); 
}

void initPerson (Person *P)
{
  P->setname = setname;
  P->getname = getname;
  P->print = fprint;


}
