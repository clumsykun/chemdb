import csv
from os.path import abspath, dirname
from .core import _DBSubstance, _Substance


class Substance(_Substance):
    """
    Wrapper of data from target substance database with identity key.

    Parameters
    ----------
    db: DBSubstance
        Target substance database.

    identity: str
        One of ('name', 'cas', 'smiles', 'formula').
    """

    def __init__(self, db: _DBSubstance, identity: str):
        super().__init__(db, identity)

    def to_dict(self) -> dict:
        return {
            'name': self.name,
            'cas': self.cas,
            'smiles': self.smiles,
            'formula': self.formula,
            'chinese': self.chinese,
        }


class DBSubstance(_DBSubstance):

    def __init__(self, identity_name: str, load_basic_substance: bool = True):

        if identity_name not in ('name', 'cas', 'smiles', 'formula'):
            raise ValueError(
                """
                except parameter 'identity_name' in ('name', 'cas', 'smiles', 'formula').
                """
            )

        super().__init__(identity_name)
        self.data: dict = {}
        
        if load_basic_substance:
            self._load_basic_substance()
            
        self.size: int

    def _load_basic_substance(self):
        with open(abspath(dirname(__file__)) + '/data/substance.csv') as f:
            reader = csv.reader(f)

            for idx, (name, cas, smiles, formula, chinese) in enumerate(reader):
                if idx == 0:
                    continue

                self.add_substance(
                    name,
                    cas = cas,
                    smiles = smiles,
                    formula = formula,
                    chinese = chinese,
                )

    def add_substance(self, name: str, cas: str, smiles: str,
                      formula: str, chinese: str) -> None:

        identity = self._add_substance(
            name,
            cas = cas,
            smiles = smiles,
            formula = formula,
            chinese = chinese,
        )
        self.data[identity] = Substance(self, identity)

    def get_substance(self, identity: str) -> Substance:
        return self.data.get(identity)
