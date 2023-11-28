from .core import BaseDBSubstance, BaseSubstance


class Substance(BaseSubstance):
    """
    Wrapper of data from target substance database with identity key.

    Args:
        db (DBSubstance): target substance database.
        key (str): identity key.
    """

    def __init__(self, db, key):
        super().__init__(db, key)

        self.cas: str
        self.smiles: str
        self.chem_name: str
        self.chem_chinese: str
        self.formula: str
        
    def to_dict(self) -> dict:
        return {
            'cas': self.cas,
            'smiles': self.smiles,
            'chem_name': self.chem_name,
            'chem_chinese': self.chem_chinese,
            'formula': self.formula,
        }


class DBSubstance(BaseDBSubstance):
    
    def __init__(self):
        super().__init__()
        self.data: dict = {}

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
        self.data[cas] = Substance(self, cas)

    def get_substance(self, identity_key: str) -> Substance:
        return self.data.get(identity_key)
