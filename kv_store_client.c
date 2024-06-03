/**
 * Client binary.
 */

#include "kv_store_client.h"
#include "kv_store.h"
#include <stdio.h>
#include <string.h>

#define HOST "localhost"

CLIENT* clnt_connect(char* host) {
  CLIENT* clnt = clnt_create(host, KVSTORE, KVSTORE_V1, "udp");
  if (clnt == NULL) {
    clnt_pcreateerror(host);
    exit(1);
  }
  return clnt;
}

int example(int input) {
  CLIENT *clnt = clnt_connect(HOST);

  int ret;
  int *result;

  result = example_1(&input, clnt);
  if (result == (int *)NULL) {
    clnt_perror(clnt, "call failed");
    exit(1);
  }
  ret = *result;
  xdr_free((xdrproc_t)xdr_int, (char *)result);

  clnt_destroy(clnt);
  
  return ret;
}

char* echo(char* input) {
  CLIENT *clnt = clnt_connect(HOST);

  char* ret;

  /* TODO */
    buf inb;
    inb.buf_len = strlen(input);
    inb.buf_val = input;
    buf *result;

    result = echo_1(&inb, clnt);

    if (result == (buf*)NULL) {
        clnt_perror(clnt, "echo call failed");
        exit(1);
    }
    ret = strndup(result->buf_val, result->buf_len);
    xdr_free((xdrproc_t)xdr_int, (buf*)result);
  clnt_destroy(clnt);
  
  return ret;
}

void put(struct kv *pair) {
  CLIENT *clnt = clnt_connect(HOST);

  /* TODO */
    void* res;
    res = put_1(pair, clnt);
    if (res == (void*) NULL) {
        clnt_perror(clnt,"put call failed");
        exit(1);
    }
    
  clnt_destroy(clnt);
}

buf* get(buf key) {
  CLIENT *clnt = clnt_connect(HOST);

  buf* ret;

  /* TODO */
    buf *res;
    res = get_1(&key, clnt);
    if (res == (buf*)NULL) {
        clnt_perror(clnt, "get call failed");
        exit(1);
    }
    ret = (buf*)malloc(sizeof(buf));
    ret->buf_val = strndup(res->buf_val, res->buf_len);
    ret->buf_len = res->buf_len;
    xdr_free((xdrproc_t)xdr_buf, (char *)res);
  clnt_destroy(clnt);
  
  return ret;
}
