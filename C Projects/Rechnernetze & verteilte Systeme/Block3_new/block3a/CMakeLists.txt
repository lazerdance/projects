cmake_minimum_required (VERSION 2.6)
add_executable(client client.c protokoll.c hashmap.c)
add_executable(server server.c  protocol.c hashmap.c)