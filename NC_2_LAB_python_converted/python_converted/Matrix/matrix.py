from __future__ import annotations
import math
from copy import deepcopy


class Matrix:
    def __init__(self, rows=None, cols=None, data=None):
        if data is not None:
            self.data = [list(row) for row in data]
            self.rows = len(self.data)
            self.cols = len(self.data[0]) if self.data else 0
            return
        if rows is None or cols is None:
            raise ValueError("rows and cols must be specified")
        if rows <= 0 or cols <= 0:
            raise ValueError("matrix dimensions must be positive")
        self.rows = rows
        self.cols = cols
        self.data = [[0.0 for _ in range(cols)] for _ in range(rows)]

    def __repr__(self):
        return "\n".join(" ".join(f"{v:8.4f}" for v in row) for row in self.data)

    def add(self, other):
        if self.rows != other.rows or self.cols != other.cols:
            raise ValueError("Addition size mismatch")
        return Matrix(data=[[a + b for a, b in zip(r1, r2)] for r1, r2 in zip(self.data, other.data)])

    def subtract(self, other):
        if self.rows != other.rows or self.cols != other.cols:
            raise ValueError("Subtraction size mismatch")
        return Matrix(data=[[a - b for a, b in zip(r1, r2)] for r1, r2 in zip(self.data, other.data)])

    def multiply(self, other):
        if self.cols != other.rows:
            raise ValueError("Multiplication size mismatch")
        out = [[0.0 for _ in range(other.cols)] for _ in range(self.rows)]
        for i in range(self.rows):
            for j in range(other.cols):
                total = 0.0
                for k in range(self.cols):
                    total += self.data[i][k] * other.data[k][j]
                out[i][j] = total
        return Matrix(data=out)

    def __add__(self, other):
        return self.add(other)

    def __sub__(self, other):
        return self.subtract(other)

    def __mul__(self, other):
        if isinstance(other, (int, float)):
            return Matrix(data=[[value * other for value in row] for row in self.data])
        return self.multiply(other)

    def transpose(self):
        return Matrix(data=list(map(list, zip(*self.data))))

    def determinant(self):
        if self.rows != self.cols:
            raise ValueError("Determinant only exists for square matrices")
        a = deepcopy(self.data)
        det = 1.0
        n = self.rows
        for k in range(n - 1):
            pivot = max(range(k, n), key=lambda i: abs(a[i][k]))
            if abs(a[pivot][k]) < 1e-12:
                return 0.0
            if pivot != k:
                a[k], a[pivot] = a[pivot], a[k]
                det *= -1.0
            for i in range(k + 1, n):
                factor = a[i][k] / a[k][k]
                for j in range(k, n):
                    a[i][j] -= factor * a[k][j]
        for i in range(n):
            det *= a[i][i]
        return det

    def inverse(self):
        if self.rows != self.cols:
            raise ValueError("Inverse only exists for square matrices")
        n = self.rows
        aug = deepcopy(self.data)
        for i in range(n):
            aug[i] += [1.0 if i == j else 0.0 for j in range(n)]
        for col in range(n):
            pivot = max(range(col, n), key=lambda r: abs(aug[r][col]))
            if abs(aug[pivot][col]) < 1e-12:
                raise ValueError("Matrix is singular")
            aug[col], aug[pivot] = aug[pivot], aug[col]
            pv = aug[col][col]
            for j in range(2 * n):
                aug[col][j] /= pv
            for r in range(n):
                if r == col:
                    continue
                factor = aug[r][col]
                for j in range(2 * n):
                    aug[r][j] -= factor * aug[col][j]
        return Matrix(data=[row[n:] for row in aug])

    def is_square(self):
        return self.rows == self.cols

    def is_identity(self):
        if not self.is_square():
            return False
        for i in range(self.rows):
            for j in range(self.cols):
                expected = 1.0 if i == j else 0.0
                if abs(self.data[i][j] - expected) > 1e-12:
                    return False
        return True

    def is_symmetric(self):
        if not self.is_square():
            return False
        for i in range(self.rows):
            for j in range(self.cols):
                if abs(self.data[i][j] - self.data[j][i]) > 1e-12:
                    return False
        return True

    @staticmethod
    def gauss_jacobi(A, b, max_iter=1000, tol=1e-6, omega=1.0):
        n = len(b)
        x = [0.0 for _ in range(n)]
        x_new = [0.0 for _ in range(n)]
        for _ in range(max_iter):
            for i in range(n):
                s = 0.0
                for j in range(n):
                    if i != j:
                        s += A.data[i][j] * x[j]
                x_jacobi = (b[i] - s) / A.data[i][i]
                x_new[i] = (1.0 - omega) * x[i] + omega * x_jacobi
            err = sum(abs(x_new[i] - x[i]) for i in range(n))
            x = x_new[:]
            if err < tol:
                break
        return x

    @staticmethod
    def gauss_seidel(A, b, max_iter=1000, tol=1e-6):
        n = len(b)
        x = [0.0 for _ in range(n)]
        for _ in range(max_iter):
            old = x[:]
            for i in range(n):
                s = 0.0
                for j in range(n):
                    if i != j:
                        s += A.data[i][j] * x[j]
                x[i] = (b[i] - s) / A.data[i][i]
            err = sum(abs(x[i] - old[i]) for i in range(n))
            if err < tol:
                break
        return x


class DataPoint:
    def __init__(self, x, fx):
        self.x = float(x)
        self.fx = float(fx)


class LagrangeInterpolation:
    def __init__(self, points):
        self.points = points

    @classmethod
    def from_file(cls, filename):
        points = []
        with open(filename, 'r', encoding='utf-8') as f:
            for line in f:
                if not line.strip():
                    continue
                values = line.strip().split()
                if len(values) != 2:
                    continue
                points.append(DataPoint(float(values[0]), float(values[1])))
        if not points:
            raise ValueError(f"No interpolation points found in {filename}")
        return cls(points)

    def basis_value(self, i, x):
        n = len(self.points)
        num = 1.0
        den = 1.0
        for j in range(n):
            if j == i:
                continue
            num *= (x - self.points[j].x)
            den *= (self.points[i].x - self.points[j].x)
        if abs(den) < 1e-15:
            raise ValueError("Duplicate x values are not allowed for interpolation")
        return num / den

    def evaluate(self, x):
        return sum(p.fx * self.basis_value(i, x) for i, p in enumerate(self.points))


class LeastSquaresLinear:
    def __init__(self, filename):
        self.points = []
        with open(filename, 'r', encoding='utf-8') as f:
            for line in f:
                if not line.strip():
                    continue
                x_str, y_str = line.strip().split()
                self.points.append((float(x_str), float(y_str)))

    def fit(self):
        n = len(self.points)
        sum_x = sum(x for x, _ in self.points)
        sum_y = sum(y for _, y in self.points)
        sum_xy = sum(x * y for x, y in self.points)
        sum_x2 = sum(x * x for x, _ in self.points)
        denom = n * sum_x2 - sum_x * sum_x
        if abs(denom) < 1e-12:
            raise ValueError("Cannot fit line because x values are degenerate")
        a = (n * sum_xy - sum_x * sum_y) / denom
        b = (sum_y - a * sum_x) / n
        return a, b

    def display_table(self):
        print("x      y")
        for x, y in self.points:
            print(f"{x:8.3f} {y:8.3f}")

    def print_summary(self):
        a, b = self.fit()
        print(f"Fitted line: y = {a:.6f}x + {b:.6f}")


if __name__ == "__main__":
    A = Matrix(rows=2, cols=2, data=[[4.0, 1.0], [2.0, 3.0]])
    B = Matrix(rows=2, cols=2, data=[[1.0, 2.0], [3.0, 4.0]])
    print("A + B =")
    print(A + B)
    print("A * B =")
    print(A * B)
    print("Determinant(A) =", A.determinant())
    print("Inverse(A) =")
    print(A.inverse())
