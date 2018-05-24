#ifndef LLARP_IWP_H_
#define LLARP_IWP_H_
#include <llarp/crypto.h>
#include <llarp/link.h>
#include <llarp/msg_handler.h>
#include <llarp/router_identity.h>

#ifdef __cplusplus
extern "C" {
#endif

struct llarp_iwp_args
{
  struct llarp_alloc* mem;
  struct llarp_crypto* crypto;
  struct llarp_logic* logic;
  struct llarp_threadpool* cryptoworker;
  const char* keyfile;
};

void
iwp_link_init(struct llarp_link* link, struct llarp_iwp_args args,
              struct llarp_msg_muxer* muxer);

#ifdef __cplusplus
}
#endif
#endif