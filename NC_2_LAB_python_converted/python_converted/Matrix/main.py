from matrix import Matrix, LagrangeInterpolation, LeastSquaresLinear

if __name__ == "__main__":
    print("         Matrix Computation")
    print("============================================")
    print("Choose Method:")
    print("  1. Gaussian Elimination")
    print("  2. LU - Crout")
    print("  3. LU - Doolittle")
    print("  4. LU - Cholesky")
    print("  5. Gauss-Jacobi")
    print("  6. Gauss-Seidel")
    print("  7. Gerschgorin Circle Theorem")
    print("  8. Lagrange Interpolation")
    print("  9. Least Squares Curve Fitting (Linear)")
    choice = input("Enter choice: ").strip()

    if choice == "8":
        filename = input("Enter data file name (e.g. Lagrange.txt): ").strip()
        lagrange = LagrangeInterpolation.from_file(filename)
        x_value = float(input("Enter x value: "))
        print(f"P({x_value}) = {lagrange.evaluate(x_value)}")
    elif choice == "9":
        filename = input("Enter data file (e.g. curve_data.txt): ").strip()
        ls = LeastSquaresLinear(filename)
        ls.display_table()
        a, b = ls.fit()
        print(f"Fitted line: y = {a:.6f}x + {b:.6f}")
    else:
        A = Matrix(rows=3, cols=3, data=[[2.0, 1.0, -1.0], [-3.0, -1.0, 2.0], [-2.0, 1.0, 2.0]])
        b = [8.0, -11.0, -3.0]
        print("Gauss-Jacobi solution:", Matrix.gauss_jacobi(A, b, max_iter=1000, tol=1e-8))
        print("Gauss-Seidel solution:", Matrix.gauss_seidel(A, b, max_iter=1000, tol=1e-8))
