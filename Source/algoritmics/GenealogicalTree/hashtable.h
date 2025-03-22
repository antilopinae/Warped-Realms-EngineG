#ifndef HASHTABLE_H
#define HASHTABLE_H

#define hashsize(n) ((unsigned int)1<<(n))
#define hashmask(n) (hashsize(n)-1)

// Key - data for hash, len - length of this data, bits - count of bits of hashcode
unsigned long oaat(char* key, unsigned long len, unsigned long bits){
  unsigned long hash, i;
  for(hash = 0, i = 0; i < len; i++){
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash & hashmask(bits);
}

typedef struct hashnode{
  struct node* data;
  struct hashnode* next;
} hashnode;


#endif //HASHTABLE_H
