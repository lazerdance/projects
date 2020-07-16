#!/usr/bin/python3
# -*- coding: utf-8 -*-


def main():
    # hardcoded udp times
    udp = [0.0155685, 0.0121505, 0.0101792, 0.0123061, 0.0123061]
    # hardcoded tcp times
    tcp = [0.00894745, 0.00728269, 0.00867837, 0.00653385, 0.00895354]

    tcp_avg = sum(tcp) / float(len(tcp))
    udp_avg = sum(udp) / float(len(udp))

    print("tcp_avg: " + str(tcp_avg))
    print("udp_avg: " + str(udp_avg))

    return


if __name__ == "__main__":
    main()
