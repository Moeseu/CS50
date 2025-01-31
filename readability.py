import sys
from cs50 import get_string

text = get_string("Text: ")
len = len(text)
cherectars = 0
stancsis = 0
words = 1

for i in text:
    if i.isalpha():
        cherectars += 1
    if i == " ":
        words += 1
    if i == "." or i == "?" or i == "!":
        stancsis += 1

L = (cherectars * 100)/words
S = (stancsis * 100)/words

index = 0.0588 * L - 0.296 * S - 15.8
if index < 1:
    print("Before Grade 1")
    sys.exit()

index = round(index)

if index > 16:
    print("Grade 16+")
    sys.exit()

print(f"Grade {index}")
