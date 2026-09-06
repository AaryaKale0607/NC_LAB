class NewtonInterpolation:
    def __init__(self, x_data, y_data):
        assert len(x_data) == len(y_data), "x_data and y_data must be the same length"
        self.x_data = list(x_data)
        self.y_data = list(y_data)
        self.n = len(x_data)
        self.table = self._build_table()
        self.coeffs = [self.table[0][j] for j in range(self.n)]
 
    def _build_table(self):
        """Construct the full divided-difference table from scratch."""
        n = self.n
        table = [[0.0] * n for _ in range(n)]
        for i in range(n):
            table[i][0] = self.y_data[i]
        for j in range(1, n):
            for i in range(n - j):
                table[i][j] = (table[i + 1][j - 1] - table[i][j - 1]) / (
                    self.x_data[i + j] - self.x_data[i]
                )
        return table
 
    def print_table(self):
        """Pretty-print the divided-difference table."""
        print("Divided-Difference Table:")
        for i in range(self.n):
            row = " ".join(f"{self.table[i][j]:10.5f}" for j in range(self.n - i))
            print(f"  x={self.x_data[i]:7.4f} : {row}")
 
    def solve(self, x):
        """Evaluate the Newton polynomial P(x), nested (Horner-like) form."""
        result = self.coeffs[self.n - 1]
        for k in range(self.n - 2, -1, -1):
            result = result * (x - self.x_data[k]) + self.coeffs[k]
        return result
 
    def differentiate(self, x):
        """
        Differentiate the Newton polynomial ALGORITHMICALLY (product
        rule applied term-by-term in a loop), not symbolically and not
        via finite differences. Returns the exact P'(x).
        """
        P = self.coeffs[0]
        dP = 0.0
        T = 1.0
        dT = 0.0
        for k in range(1, self.n):
            dT = dT * (x - self.x_data[k - 1]) + T
            T = T * (x - self.x_data[k - 1])
            P += self.coeffs[k] * T
            dP += self.coeffs[k] * dT
        return dP
 