import os
import re
import time
import shlex
import shutil
import string
import random
import subprocess as sp
from timeit import default_timer as timer

peer_ids = []
peer_processes = []
data = {}
deleted = []
HASHSIZE = 128
FINGERSIZE = 7


def calculate_fingertable():
    with open("./testlogs/fingertables.txt", "w") as file:
        peer_ids.sort()
        for i in peer_ids:
            file.write("=== Fingertable of peer %d ===\n" % i)
            for j in range(FINGERSIZE):
                start = (2 ** j + i) % (2 ** FINGERSIZE)
                nodeid = peer_ids[(peer_ids.index(i) + 1) % len(peer_ids)]
                for k in peer_ids:
                    if k >= start:
                        nodeid = k
                        break
                if start > nodeid:
                    nodeid = peer_ids[0]
                file.write("Eintrag %d Start %d Node %d\n" % (j, start, nodeid))
    return


def start_peers(n: int):
    # IDs bis 65536 müssen möglich sein
    start_peer = 'valgrind --leak-check=full --show-leak-kinds=all ../peer128 localhost '
    for i in range(n):
        connect_to = ""
        negate = 0
        if i > 0:
            connect_to = " localhost %d" % (4000 + random.randint(0, i - 1))
            negate = -1
        deviation = HASHSIZE // (4 * n)
        nodeid = (HASHSIZE * i // n) + random.randint(negate * deviation, deviation)
        command = start_peer + "%d %d%s" % (4000 + i, nodeid, connect_to)
        with open("./testlogs/peer%dstdout.txt" % (4000 + i), "w") as stdoutFile:
            with open("./testlogs/peer%dstderr.txt" % (4000 + i), "w") as stderrFile:
                peer = sp.Popen(shlex.split(command),
                                stdin=sp.DEVNULL,
                                stdout=stdoutFile,
                                stderr=stderrFile,
                                bufsize=1)
        peer_ids.append(nodeid)
        peer_processes.append(peer)
        print(command)
        time.sleep(0.1 * (i // 10 + 1))
    return


def send_query(n: int, query: str) -> dict:
    port = 4000 + random.randint(0, n - 1)
    command = "../client localhost %d %s" % (port, query)
    peer = sp.Popen(shlex.split(command),
                    stdin=sp.DEVNULL,
                    stdout=sp.PIPE,
                    stderr=sp.DEVNULL,
                    bufsize=1)
    answer = str(peer.stdout.read(), "utf-8")
    result = {"port": port}
    if "Can't connect to host." in answer:
        return result
    splitted = answer.split("\n")
    transid = re.search(r"\d+", splitted[0]).group()
    for element in splitted[1:-1]:
        pair = element.split(":")
        if (len(pair) < 2):
            pair = element.split(" ")
        if (len(pair) > 1):
            result[pair[0].strip()] = pair[1].strip()
    if result["TransID"] != transid:
        print("Received TransID is wrond by query '%s'" % query)
    return result


def test_peers(n: int):
    k = -1
    while k == -1:
        nstr = input("Will test k times. k: ")
        try:
            k = int(nstr)
            if k < 50:
                print("k should be greater than 50.")
                k = -1
        except ValueError:
            print("'%s' is not a number." % nstr)

    # log = open("./testlogs/querys.txt", "w")
    for i in range(k):
        if i % 50 == 0 and i > 0:
            print("Passed test %d" % i)

        operation = random.randint(0, 8)
        if len(data) < 10:
            operation = 0
        if len(data) > 600:
            operation = 4
        if len(deleted) == 0 and (operation == 6 or operation == 7):
            operation = 4

        query = ""
        answer = {}
        if operation == 0 or operation == 1:
            key = ''.join(random.choice(string.ascii_lowercase) for j in range(20))
            value = ''.join(random.choice(string.ascii_lowercase) for j in range(20))
            query = "SET %s %s" % (key, value)
            data[key] = value
            answer = send_query(n, query)
            if len(answer) == 1:
                print("Can't reach peer %d anymore." % answer["port"])
                # log.close()
                return
            if answer["Ack"] != "1" or answer["Set"] != "1":
                print("Query '%s' failed." % query)
                # log.close()
                return
        elif operation == 2 or operation == 3:
            key = random.choice(list(data.keys()))
            value = data[key]
            query = "GET %s" % key
            answer = send_query(n, query)
            if len(answer) == 1:
                print("Can't reach peer %d anymore." % answer["port"])
                # log.close()
                return
            if answer["Ack"] != "1" or answer["Get"] != "1" or answer["Value"] != value:
                print("Query '%s' failed." % query)
                # log.close()
                return
        elif operation == 4 or operation == 5:
            key = random.choice(list(data.keys()))
            del data[key]
            deleted.append(key)
            query = "DELETE %s" % key
            answer = send_query(n, query)
            if len(answer) == 1:
                print("Can't reach peer %d anymore." % answer["port"])
                # log.close()
                return
            if answer["Ack"] != "1" or answer["Del"] != "1":
                print("Query '%s' failed." % query)
                # log.close()
                return
        elif operation == 6:
            key = random.choice(deleted)
            query = "GET %s" % key
            answer = send_query(n, query)
            if len(answer) == 1:
                print("Can't reach peer %d anymore." % answer["port"])
                # log.close()
                return
            if answer["Ack"] != "1" or answer["Get"] != "0" or answer["Value"] != "":
                print("Query '%s' failed." % query)
                # log.close()
                return
        elif operation == 7:
            key = random.choice(deleted)
            query = "DELETE %s" % key
            answer = send_query(n, query)
            if len(answer) == 1:
                print("Can't reach peer %d anymore." % answer["port"])
                # log.close()
                return
            if answer["Ack"] != "1" or answer["Del"] != "0":
                print("Query '%s' failed." % query)
                # log.close()
                return

        # log.write("OP: %d\tPort: %d\tQuery: %s\n" % (operation, answer["port"], query))
    # log.close()
    return


def terminate_peers():
    for peer in peer_processes:
        peer.terminate()
    print("Peers closed.")
    return


def check_memory():
    files = os.listdir("./testlogs/")
    k = 0

    for filename in files:
        if re.fullmatch(r"peer4[0-1][0-9][0-9]stderr.txt", filename):
            k += 1
            feedback = ""
            with open("./testlogs/" + filename, "r") as file:
                feedback = file.read()

            match = re.search(r"in use at exit: [0-9]* bytes in [0-9]* blocks", feedback)
            lost_bytes = int(re.search(r"\d+", match.group()).group())
            if lost_bytes != 0:
                peername = re.search(r"peer4[0-1][0-9][0-9]", filename).group()
                print("Memory error by peer %s." % peername)

            match = re.search(r"ERROR SUMMARY: [0-9]* errors from [0-9]* contexts", feedback)
            error_amount = int(re.search(r"\d+", match.group()).group())
            if error_amount != 0:
                peername = re.search(r"peer4[0-1][0-9][0-9]", filename).group()
                print("Error by peer %s." % peername)

    print("Checked memory of %d peers." % k)
    return


def main():
    if not os.path.exists("../client") or not os.path.exists("../peer"):
        print("Need client and peer in parent folder!")
        return
    if os.path.isdir("./testlogs"):
        shutil.rmtree("./testlogs")
    os.mkdir("./testlogs")

    n = -1
    while n == -1:
        nstr = input("Will start n peers. n: ")
        try:
            n = int(nstr)
            if n < 0:
                print("n should be positiv.")
                n = -1
            if n > 128:
                print("n should be lower than 128.")
                n = -1
        except ValueError:
            print("'%s' is not a number." % nstr)
    print("Start %d peers." % n)
    start_peers(n)

    answer = input("Calculate fingertable? (y/n) ")
    if answer == "y":
        calculate_fingertable()

    answer = input("Test peers randomized? (y/n) ")
    while answer == "y":
        start = timer()
        test_peers(n)
        end = timer()
        print("Elapsed time: ", end - start)
        answer = input("Test again? (y/n) ")

    input("Press enter to terminate peers.")
    terminate_peers()

    answer = input("Check for memory errors? (y/n) ")
    if answer == "y":
        check_memory()
    return


if __name__ == "__main__":
    main()
