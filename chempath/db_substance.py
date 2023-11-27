from .core import BaseDBSubstance, BaseSubstance


class DBSubstance(BaseDBSubstance):
    
    def __init__(self):
        super().__init__()
        self.cache: dict = {}

    @property
    def size(self) -> int:
        return super().size

    def add_substance(self, cas: str, smiles: str,
                      chem_name: str, chem_chinese: str,
                      formula: str) -> None:
        super().add_substance(
            cas,
            smiles = smiles,
            name = chem_name,
            chinese = chem_chinese,
            formula = formula,
        )
        self.cache[cas] = BaseSubstance(self, cas)
