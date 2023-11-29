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

    def __init__(self, db, identity):
        super().__init__(db, identity)

        self.name: str
        self.cas: str
        self.smiles: str
        self.formula: str
        self.chinese: str

    def to_dict(self) -> dict:
        return {
            'name': self.name,
            'cas': self.cas,
            'smiles': self.smiles,
            'formula': self.formula,
            'chinese': self.chinese,
        }


class DBSubstance(_DBSubstance):

    def __init__(self, identity_name: str = "cas", load_basic_substance: bool = True):
        super().__init__(identity_name)
        self.data: dict = {}
        
        if load_basic_substance:
            self._load_basic_substance()

    def _load_basic_substance(self):
        with open(abspath(dirname(__file__)) + '/data/substance.csv') as f:
            reader = csv.reader(f)

            for idx, (name, cas, smiles, formula, chinese) in enumerate(reader):
                if idx == 0:
                    continue

                self.add_substance(
                    name,
                    cas = cas,
                    smiles=smiles,
                    formula=formula,
                    chinese=chinese,
                )

    @property
    def size(self) -> int:
        return super().size

    def add_substance(self, name: str, 
                      cas: str, smiles: str, formula: str, chinese: str) -> None:

        super().add_substance(
            name,
            cas = cas,
            smiles = smiles,
            formula = formula,
            chinese = chinese,
        )
        self.data[cas] = Substance(self, cas)

    def get_substance(self, identity_key: str) -> Substance:
        return self.data.get(identity_key)
