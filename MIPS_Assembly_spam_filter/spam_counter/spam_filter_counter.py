import sys

SUBJECT = b"Subject: "
HEADER_END = b"\r\n\r\n"

if len(sys.argv) != 3:
    print("usage: python aufgabe2.py <email file> <badwords file>")
    sys.exit(-1)

def find_pos(email):
    return map(
            lambda x: email.find(x) + len(x),
            (SUBJECT, HEADER_END)
            )

def read_badwords(filename):
    with open(filename) as f:
        return f.read().split(',')

with open(sys.argv[1], "rb") as f:
    email = f.read()

    subject_pos, header_end_pos = find_pos(email)
    bw_it = iter(read_badwords(sys.argv[2]))
    badwords = map(lambda x: (x, int(next(bw_it))), bw_it)

    score = 0
    body = email[header_end_pos:].decode("utf-8")
    print(body)

    for word,value in badwords:
        last_pos = 0
        while True:
            last_pos = body.find(word, last_pos)
            if last_pos < 0:
                break
            score += value
            last_pos += 1

    print("Der Spamscore betraegt {}".format(score))
