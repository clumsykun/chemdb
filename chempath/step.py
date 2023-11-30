from .core import _DBStep
from .substance import DBSubstance


class DBStep(_DBStep):
    
    def __init__(self, db: DBSubstance) -> None:
        super().__init__(db)
        # pass

