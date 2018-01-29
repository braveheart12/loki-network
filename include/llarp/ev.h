#ifndef LLARP_EV_H
#define LLARP_EV_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

struct llarp_ev_loop;

void llarp_ev_loop_alloc(struct llarp_ev_loop **ev);
void llarp_ev_loop_free(struct llarp_ev_loop **ev);

int llarp_ev_loop_run(struct llarp_ev_loop *ev);
/** stop event loop and wait for it to complete all jobs */
void llarp_ev_loop_stop(struct llarp_ev_loop *ev);

struct llarp_udp_listener {
  char *host;
  uint16_t port;
  void *user;
  void *impl;
  void (*recvfrom)(struct llarp_udp_listener *, const struct sockaddr *, char *,
                   ssize_t);
  void (*closed)(struct llarp_udp_listener *);
};

int llarp_ev_add_udp_listener(struct llarp_ev_loop *ev,
                              struct llarp_udp_listener *listener);

int llarp_ev_close_udp_listener(struct llarp_udp_listener *listener);

struct llarp_ev_job {
  /** the loop this job belongs to */
  struct llarp_ev_loop *loop;
  /** user data */
  void *user;
  /** work is called async when ready in the event loop thread */
  void (*work)(struct llarp_ev_job *);
};

/**
    call work async in event loop thread (thread safe)
    return true if we queued the job otherwise return false
 */
bool llarp_ev_async(struct llarp_ev_loop *ev, struct llarp_ev_job job);

#ifdef __cplusplus
}
#endif
#endif