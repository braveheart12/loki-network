set(LIB_UTIL_SRC
  config/config.cpp
  config/ini.cpp
  constants/defaults.cpp
  constants/link_layer.cpp
  constants/path.cpp
  constants/proto.cpp
  constants/version.cpp
  util/aligned.cpp
  util/bencode.cpp
  util/bits.cpp
  util/buffer.cpp
  util/codel.cpp
  util/common.cpp
  util/encode.cpp
  util/endian.cpp
  util/fs.cpp
  util/json.cpp
  util/logger.cpp
  util/android_logger.cpp
  util/file_logger.cpp
  util/logic.cpp
  util/loglevel.cpp
  util/mem.cpp
  util/memfn_traits.cpp
  util/memfn.cpp
  util/metrics_core.cpp
  util/metrics_types.cpp
  util/json_logger.cpp
  util/ostream_logger.cpp
  util/syslog_logger.cpp
  util/win32_logger.cpp
  util/metrics.cpp
  util/object.cpp
  util/printer.cpp
  util/queue_manager.cpp
  util/queue.cpp
  util/scheduler.cpp
  util/status.cpp
  util/stopwatch.cpp
  util/str.cpp
  util/string_view.cpp
  util/thread_pool.cpp
  util/threading.cpp
  util/threadpool.cpp
  util/time.cpp
  util/timer.cpp
  util/timerqueue.cpp
  util/traits.cpp
  util/types.cpp
  util/variant.cpp
)

add_library(${UTIL_LIB} STATIC ${LIB_UTIL_SRC})
target_include_directories(${UTIL_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${UTIL_LIB} PUBLIC ${CRYPTOGRAPHY_LIB} ${FS_LIB})
target_link_libraries_system(${UTIL_LIB} absl::synchronization absl::hash absl::container nlohmann_json::nlohmann_json)

# cut back on fluff
if (NOT WIN32)
  target_link_libraries_system(${UTIL_LIB} absl::optional absl::variant absl::strings)
endif(NOT WIN32)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(ISOLATE_PROC_SRC linux/netns.cpp)
endif()

set(LIB_PLATFORM_SRC
# for networking
  ev/ev.cpp
  ev/pipe.cpp
  metrics/metrictank_publisher.cpp
  metrics/json_publisher.cpp
  metrics/stream_publisher.cpp
  net/net.cpp
  net/net_addr.cpp
  net/net_inaddr.cpp
# for android shim
  ${ANDROID_PLATFORM_SRC}
# process isolation implementation
  ${ISOLATE_PROC_SRC}
# tun
  ${LIBTUNTAP_SRC}
  ${EV_SRC}
)

if (WIN32)
  set(LIB_PLATFORM_SRC
    ${LIB_PLATFORM_SRC}
    win32/win32_inet.c
    win32/win32_intrnl.c
    win32/win32_upoll.c)
else()
  set(LIB_PLATFORM_SRC
      ${LIB_PLATFORM_SRC}
      ev/ev_libuv.cpp)
endif(WIN32)

add_library(${PLATFORM_LIB} STATIC ${LIB_PLATFORM_SRC})
target_link_libraries(${PLATFORM_LIB} PUBLIC ${CRYPTOGRAPHY_LIB} ${UTIL_LIB} libutp Threads::Threads ${LIBS})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  if(NON_PC_TARGET)
    add_import_library(rt)
    target_link_libraries(${PLATFORM_LIB} PUBLIC rt)
  else()
    add_import_library(cap)
    target_link_libraries(${PLATFORM_LIB} PUBLIC cap)
  endif()
endif()

if(WIN32)
  target_link_libraries(${PLATFORM_LIB} PUBLIC iphlpapi)
endif()
set(DNSLIB_SRC
  dns/dns.cpp
  dns/message.cpp
  dns/name.cpp
  dns/query.cpp
  dns/question.cpp
  dns/rectypes.cpp
  dns/rr.cpp
  dns/serialize.cpp
  dns/server.cpp
  dns/string.cpp
)

set(LIB_SRC
  ${DNSLIB_SRC}
  context.cpp
  crypto/constants.cpp
  crypto/crypto_libsodium.cpp
  crypto/crypto_noop.cpp
  crypto/crypto.cpp
  crypto/ec.cpp
  crypto/encrypted_frame.cpp
  crypto/encrypted.cpp
  crypto/types.cpp
  dht/bucket.cpp
  dht/context.cpp
  dht/dht.cpp
  dht/explorenetworkjob.cpp
  dht/kademlia.cpp
  dht/key.cpp
  dht/localtaglookup.cpp
  dht/localrouterlookup.cpp
  dht/localserviceaddresslookup.cpp
  dht/message.cpp
  dht/messages/findintro.cpp
  dht/messages/findrouter.cpp
  dht/messages/gotintro.cpp
  dht/messages/gotrouter.cpp
  dht/messages/pubintro.cpp
  dht/node.cpp
  dht/publishservicejob.cpp
  dht/recursiverouterlookup.cpp
  dht/serviceaddresslookup.cpp
  dht/taglookup.cpp
  dht/tx.cpp
  dht/txholder.cpp
  dht/txowner.cpp
  dns.cpp
  dnsc.cpp
  dnsd.cpp
  exit/context.cpp
  exit/endpoint.cpp
  exit/exit_messages.cpp
  exit/policy.cpp
  exit/session.cpp
  handlers/exit.cpp
  handlers/null.cpp
  handlers/tun.cpp
  hook/shell.cpp
  iwp/linklayer.cpp
  iwp/outermessage.cpp
  iwp/iwp.cpp
  link/server.cpp
  link/session.cpp
  messages/dht_immediate.cpp
  messages/discard.cpp
  messages/link_intro.cpp
  messages/link_message_parser.cpp
  messages/link_message.cpp
  messages/relay.cpp
  messages/relay_commit.cpp
  net/address_info.cpp
  net/exit_info.cpp
  net/ip.cpp
  net/net_int.cpp
  nodedb.cpp
  path/ihophandler.cpp
  path/path_context.cpp
  path/path_types.cpp
  path/path.cpp
  path/pathbuilder.cpp
  path/pathset.cpp
  path/transit_hop.cpp
  pow.cpp
  profiling.cpp
  router/abstractrouter.cpp
  router/router.cpp
  router_contact.cpp
  router_id.cpp
  routing/dht_message.cpp
  routing/handler.cpp
  routing/message_parser.cpp
  routing/message.cpp
  routing/path_confirm_message.cpp
  routing/path_latency_message.cpp
  routing/path_transfer_message.cpp
  routing/transfer_traffic_message.cpp
  rpc/rpc.cpp
  service/address.cpp
  service/async_key_exchange.cpp
  service/config.cpp
  service/context.cpp
  service/endpoint_util.cpp
  service/endpoint.cpp
  service/handler.cpp
  service/hidden_service_address_lookup.cpp
  service/identity.cpp
  service/info.cpp
  service/intro_set.cpp
  service/intro.cpp
  service/lookup.cpp
  service/outbound_context.cpp
  service/pendingbuffer.cpp
  service/protocol.cpp
  service/sendcontext.cpp
  service/session.cpp
  service/tag_lookup_job.cpp
  service/tag.cpp
  service/vanity.cpp
  utp/inbound_message.cpp
  utp/linklayer.cpp
  utp/session.cpp
  utp/utp.cpp
)
if(TESTNET)
  set(LIB_SRC ${LIB_SRC} testnet.c)
endif()

add_library(${STATIC_LIB} STATIC ${LIB_SRC})
set(LIBS ${LIBS} libutp)
target_link_libraries(${STATIC_LIB} PUBLIC cxxopts ${ABYSS_LIB} ${PLATFORM_LIB} ${UTIL_LIB} ${CRYPTOGRAPHY_LIB} ${FS_LIB})

if(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  target_include_directories(${PLATFORM_LIB} SYSTEM PUBLIC /usr/local/include)
  target_include_directories(${STATIC_LIB} SYSTEM PUBLIC /usr/local/include)
endif()

if(WITH_SHARED)
  add_library(${SHARED_LIB} SHARED ${LIB_SRC})
  set(LIBS ${LIBS} Threads::Threads)
  target_link_libraries(${SHARED_LIB} PUBLIC ${ABYSS_LIB} ${CRYPTOGRAPHY_LIB} ${UTIL_LIB} ${PLATFORM_LIB} ${LIBS})
  if (WIN32)
    target_link_libraries(${SHARED_LIB} PUBLIC ${FS_LIB} ws2_32 iphlpapi)
  else()
    install(TARGETS ${SHARED_LIB} LIBRARY DESTINATION lib)
  endif()
  add_log_tag(${SHARED_LIB})
endif()

if (WARNINGS_AS_ERRORS)
  set(WARN_FLAGS -Wall -Wextra -Wshadow -Werror)
  target_compile_options(${UTIL_LIB} PUBLIC ${WARN_FLAGS})
  target_compile_options(${PLATFORM_LIB} PUBLIC ${WARN_FLAGS})
  target_compile_options(${STATIC_LIB} PUBLIC ${WARN_FLAGS})
endif()
add_log_tag(${UTIL_LIB})
add_log_tag(${PLATFORM_LIB})
add_log_tag(${STATIC_LIB})
