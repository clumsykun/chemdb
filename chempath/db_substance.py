from .core import BaseDBSubstance


class DBSubstance(BaseDBSubstance):
    
    def __init__(self):
        super().__init__()

        self.size: int
