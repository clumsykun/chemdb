import unittest
from chempath.db_substance import *


class TestDBSubstance(unittest.TestCase):

    def setUp(self):
        self.db = DBSubstance()

    def test_add(self):
        self.db.add_substance(
            '10094-36-7',
            smiles = 'C(CC(OCC)=O)C1CCCCC1',
            chem_name = 'Ethyl cyclohexanepropionate',
            chem_chinese = None,
            formula = 'C[11]H[20]O[2]',
        )
        self.assertEqual(self.db.size, 1)
