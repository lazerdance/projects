import sys

SUBJECT = b"Subject: "
HEADER_END = b"\r\n\r\n"

pr = sys.stdout.write

if len(sys.argv) < 2:
    print("usage: python aufgabe3.py <email-file> <spam>")
    sys.exit(-1)

def print_email(email, pos, spam):
    pr(email[0:pos].decode("utf-8"))
    if spam:
        pr("[SPAM] ")
    pr(email[pos:].decode("utf-8"))

def find_pos(email):
    return map(
            lambda x: email.find(x) + len(x),
            (SUBJECT, HEADER_END)
            )

with open(sys.argv[1], "rb") as file:
    email = file.read()

    subject_pos, header_end_pos = find_pos(email)
    print_email(email, subject_pos, len(sys.argv)>2)
