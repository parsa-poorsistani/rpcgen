/**
 * Server binary.
 */

#include "kv_store.h"
#include <glib.h>
#include <memory.h>
#include <netinet/in.h>
#include <rpc/pmap_clnt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#ifndef SIG_PF
#define SIG_PF void (*)(int)
#endif

/* TODO: Add global state. */

GHashTable* kv_store;

extern void kvstore_1(struct svc_req *, SVCXPRT *);

/* Set up and run RPC server. */
int main(int argc, char **argv) {
  register SVCXPRT *transp;

  pmap_unset(KVSTORE, KVSTORE_V1);

  transp = svcudp_create(RPC_ANYSOCK);
  if (transp == NULL) {
    fprintf(stderr, "%s", "cannot create udp service.");
    exit(1);
  }
  if (!svc_register(transp, KVSTORE, KVSTORE_V1, kvstore_1, IPPROTO_UDP)) {
    fprintf(stderr, "%s", "unable to register (KVSTORE, KVSTORE_V1, udp).");
    exit(1);
  }

  transp = svctcp_create(RPC_ANYSOCK, 0, 0);
  if (transp == NULL) {
    fprintf(stderr, "%s", "cannot create tcp service.");
    exit(1);
  }
  if (!svc_register(transp, KVSTORE, KVSTORE_V1, kvstore_1, IPPROTO_TCP)) {
    fprintf(stderr, "%s", "unable to register (KVSTORE, KVSTORE_V1, tcp).");
    exit(1);
  }

  /* TODO: Initialize state. */

  kv_store = g_hash_table_new(g_str_hash, g_str_equal);
  svc_run();
  fprintf(stderr, "%s", "svc_run returned");
  exit(1);
  /* NOTREACHED */
}

/* Example server-side RPC stub. */
int *example_1_svc(int *argp, struct svc_req *rqstp) {
  static int result;

  result = *argp + 1;

  return &result;
}

/* TODO: Add additional RPC stubs. */

buf *echo_1_svc(buf *argp, struct svc_req *rqstp) {
    static buf msg;

    if (msg.buf_val != NULL) {
        free(msg.buf_val);
    }

    msg.buf_val = strdup(argp->buf_val);
    msg.buf_len = argp->buf_len;

    return &msg;
}

void *put_1_svc(struct kv *argp, struct svc_req *rqstp) {
   char *k = strndup(argp->key.buf_val, argp->key.buf_len);
   char *v = strndup(argp->value.buf_val, argp->value.buf_len);

    g_hash_table_insert(kv_store, k, v);

    static char dummy;
    return &dummy;
}

buf *get_1_svc(buf *argp, struct svc_req *rqstp) {
    static buf res;

    if (res.buf_val != NULL) {
        free(res.buf_val);
    }

    char *k = strndup(argp->buf_val, argp->buf_len);
    char *v = g_hash_table_lookup(kv_store, k);

    if (v != NULL) {
        res.buf_val = v;
        res.buf_len = strlen(v);
    } else {
        res.buf_val = "";
        res.buf_len = 0;
    }
    
    return &res;
}
