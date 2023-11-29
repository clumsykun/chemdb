from .core import _Element, num_element


class Element(_Element):
    
    def __init__(self, id):
        super().__init__(id)

        self.id: int
        self.name: str
        self.chinese: str
        self.symbol: str
        self.mass: float


PERIODIC_TABLE = {
    v.symbol: v for v in [
        Element(i+1)
        for i in range(num_element())
    ]
}


__all__ = [
    'PERIODIC_TABLE',
    'Element',
]
