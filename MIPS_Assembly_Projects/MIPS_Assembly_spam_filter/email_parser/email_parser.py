import sys

SUBJECT = b"Subject: "
HEADER_END = b"\r\n\r\n"

def find_pos(email):
    return map(
            lambda x: email.find(x) + len(x),
            (SUBJECT, HEADER_END)
            )

with open(sys.argv[1], "rb") as f:
    email = f.read()
    subject_pos, header_end_pos = find_pos(email)
    print(subject_pos, header_end_pos)
