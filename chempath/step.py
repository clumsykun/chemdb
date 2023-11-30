from typing import Sequence
from .core import _DBStep
from .substance import DBSubstance


class DBStep(_DBStep):
    
    def __init__(self, db_substance: DBSubstance) -> None:        
        super().__init__(db_substance)

        self.db_substance: DBSubstance

    def add_step(self, target: DBSubstance, inputs: Sequence[DBSubstance]) -> None:
        self._add_step(target, inputs)
