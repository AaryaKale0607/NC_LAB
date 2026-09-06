from complex_number import MyComplex

if __name__ == "__main__":
    c1 = MyComplex(3, 4)
    c2 = MyComplex(1, 2)
    print(f"Addition : {c1.add(c2)}")
    print(f"Multiplication : {c1.multiply(c2)}")
    print(f"Division : {c1.divide(c2)}")
    print(f"Conjugate of c1 : {c1.conjugate()}")
    print(f"Norm of c1 : {c1.norm()}")
