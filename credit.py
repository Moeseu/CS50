from cs50 import get_int

while True:
    number = get_int("Number: ")
    if number > 0:
        break
digit = len(str(abs(number)))
sum = 0

for i in range(digit, -1, -1):
    current_digit = (number // (10 ** i)) % 10
    if i % 2 != 0:
        temp = current_digit * 2
        sum += temp // 10 + temp % 10
    else:
        sum += current_digit

while number > 99:
    number //= 10
if sum % 10 == 0:
    if digit == 15 and (number == 34 or number == 37):
        print("AMEX")
    elif digit == 16 and 50 < number < 56:
        print("MASTERCARD")
    elif (digit == 16 and number // 10 == 4) or (digit == 13 and number // 10 == 4):
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
