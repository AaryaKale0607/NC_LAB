from numerical_differentiation import Differentiation, CentralDifference
 
 
class RichardsonExtrapolation(Differentiation):
    def __init__(self):
        self._central = CentralDifference()
 
    def solve(self, x, h):
        D_h = self._central.solve(x, h)
        D_h2 = self._central.solve(x, h / 2)
        return (4 * D_h2 - D_h) / 3
 