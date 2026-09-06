import math


class MyComplex:
    def __init__(self, x=0.0, y=0.0):
        self.a = float(x)
        self.b = float(y)

    def add(self, other):
        return MyComplex(self.a + other.a, self.b + other.b)

    def multiply(self, other):
        return MyComplex(
            self.a * other.a - self.b * other.b,
            self.a * other.b + self.b * other.a,
        )

    def divide(self, other):
        den = other.a * other.a + other.b * other.b
        if den == 0:
            raise ZeroDivisionError("Cannot divide by a zero complex number.")
        return MyComplex(
            (self.a * other.a + self.b * other.b) / den,
            (self.b * other.a - self.a * other.b) / den,
        )

    def conjugate(self):
        return MyComplex(self.a, -self.b)

    def norm(self):
        return math.sqrt(self.a * self.a + self.b * self.b)

    def __str__(self):
        return f"{self.a} + {self.b}i"


if __name__ == "__main__":
    c1 = MyComplex(3, 4)
    c2 = MyComplex(1, 2)
    print(f"Addition : {c1.add(c2)}")
    print(f"Multiplication : {c1.multiply(c2)}")
    print(f"Division : {c1.divide(c2)}")
    print(f"Conjugate of c1 : {c1.conjugate()}")
    print(f"Norm of c1 : {c1.norm()}")
