import unittest
from chempath.substance import *


class TestDBSubstance(unittest.TestCase):

    def setUp(self):
        self.db = DBSubstance()

    def test_add(self):
        
        cas = '10094-36-7'
        smiles = 'C(CC(OCC)=O)C1CCCCC1'
        chem_name = 'Ethyl cyclohexanepropionate'
        chem_chinese = '环己基丙酸乙酯'
        formula = 'C[11]H[20]O[2]'

        self.db.add_substance(
            cas = cas,
            smiles = smiles,
            chem_name = chem_name,
            chem_chinese = chem_chinese,
            formula = formula,
        )
        self.assertEqual(self.db.size, 1)
        substance = self.db.get_substance('10094-36-7')
        self.assertEqual(substance.cas, cas)
        self.assertEqual(substance.smiles, smiles)
        self.assertEqual(substance.chem_name, chem_name)
        self.assertEqual(substance.chem_chinese, chem_chinese)
        self.assertEqual(substance.formula, formula)

        print(self.db)
        print(self.db.data)
        print(substance.to_dict())
