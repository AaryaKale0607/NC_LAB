class LagrangeInterpolation:
    def __init__(self, x_data, y_data):
        assert len(x_data) == len(y_data), "x_data and y_data must be the same length"
        self.x_data = list(x_data)
        self.y_data = list(y_data)
        self.n = len(x_data)
 
    def basis(self, i, x):
        """Compute the i-th Lagrange basis polynomial L_i(x)."""
        xi = self.x_data[i]
        result = 1.0
        for j in range(self.n):
            if j != i:
                xj = self.x_data[j]
                result *= (x - xj) / (xi - xj)
        return result
 
    def solve(self, x):
        """Evaluate the Lagrange interpolating polynomial P(x)."""
        total = 0.0
        for i in range(self.n):
            total += self.y_data[i] * self.basis(i, x)
        return total
 
    def differentiate(self, x, h=1e-6):
        """
        Derivative of P(x), obtained with a tiny central difference on
        the interpolating polynomial itself. For an exact, purely
        algorithmic (non-finite-difference) derivative, see
        NewtonInterpolation.differentiate() in newton.py.
        """
        return (self.solve(x + h) - self.solve(x - h)) / (2 * h)
 