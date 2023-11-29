class _Substance:
    name: str
    cas: str
    smiles: str
    formula: str
    chinese: str


class _DBSubstance:

    def _add_substance(self, name: str, cas: str, smiles: str,
                       formula: str, chinese: str) -> str:
        ...
