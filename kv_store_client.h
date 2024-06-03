/**
 * Client binary.
 *
 * Do not modify this file.
 */

#include "kv_store.h"

CLIENT* clnt_connect(char* host);
int example(int input);
char* echo(char* input);
void put(struct kv *pair);
buf* get(buf key);
