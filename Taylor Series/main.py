print("Welcome to the Taylor Series Calculator.")
print("This program will calculated the Taylor Series approximations")
print("For e^x, sin(x), and cos(x) for any value of x and accuracy.")
# accuracy = float(input("Please enter an error tolerance: "))
accuracy = 0.00001
number = float(input("Please enter a number to find the exponent, sine, and cosine of: "))
oldExponent = 1
power = 1
factorial = 1
factorialMultiplier = 1
newExponent = oldExponent + (1 / factorial) * (number ** power)
exponentDifference = newExponent - oldExponent
# Find exponent
while exponentDifference > accuracy:
    factorialMultiplier = factorialMultiplier + 1
    factorial = factorial * factorialMultiplier
    power = power + 1
    oldExponent = newExponent
    newExponent = oldExponent + (1 / factorial) * (number ** power)
    exponentDifference = abs(newExponent - oldExponent)
# find sine
oldSine = 1 * number
power = 3
factorial = 3 * 2
factorialMultiplier = 3
newSine = oldSine - (1 / factorial) * (number ** power)
sineDifference = abs(newSine - oldSine)
sign = 1
while sineDifference > accuracy:
    for i in range(2):
        factorialMultiplier = factorialMultiplier + 1
        factorial = factorial * factorialMultiplier
    power = power + 2
    oldSine = newSine
    newSine = oldSine + sign * (1 / factorial) * (number ** power)
    sign = 0 - sign
    sineDifference = abs(newSine - oldSine)
# find cosine
oldCosine = 1
power = 2
factorial = 2
factorialMultiplier = 2
newCosine = oldCosine - (1 / factorial) * (number ** power)
cosineDifference = abs(newCosine - oldCosine)
sign = 1
while cosineDifference > accuracy:
    for i in range(2):
        factorialMultiplier = factorialMultiplier + 1
        factorial = factorial * factorialMultiplier
    power = power + 2
    oldCosine = newCosine
    newCosine = oldCosine + sign * (1 / factorial) * (number ** power)
    sign = 0 - sign
    cosineDifference = abs(newCosine - oldCosine)
# Report results
print("e^" + str(number) + " = " + str(newExponent))
print("sin(" + str(number) + ") = " + str(newSine))
print("cos(" + str(number) + ") = " + str(newCosine))